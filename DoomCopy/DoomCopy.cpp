#include "pch.h"
#include "DoomCopy.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef GDI_RENDER
    renderer = GDIRenderer();
#endif

#ifdef D2_RENDER
    renderer = D2Renderer();
#endif

    mainGame = Game();
    InitLogSystem(true, false);

    if (!SetupAndCreateWindow(hInstance, nCmdShow)) return -1;
    if (IS_ERROR(renderer.InitRenderer(mainHWND))) { Sleep(5000); return -1; }
    return MainLoop();
}

BOOL SetupAndCreateWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEX wcex = {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        LoadIcon(hInstance, IDI_APPLICATION),
        LoadCursor(nullptr, IDC_ARROW),
        NULL,
        NULL,
        szWindowClass,
        LoadIcon(hInstance, IDI_APPLICATION)
    };

    RegisterClassExW(&wcex);

    hInst = hInstance; // Store instance handle in our global variable

    mainHWND = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!mainHWND) return FALSE;

    ShowWindow(mainHWND, nCmdShow);
    UpdateWindow(mainHWND);
    return TRUE;
}

int MainLoop()
{
    MSG msg;
    mainGame.InitUpdate();

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                mainGame.~Game();
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        mainGame.MainUpdate();
        renderer.ProcessGame(mainHWND, &mainGame);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
            break;

        case WM_KEYDOWN:
            mainGame.CaptureKeyPress(wParam);
            break;

        case WM_KEYUP:
            mainGame.CaptureKeyRelease(wParam);
            break;

        case WM_PAINT:
            renderer.RenderScreen(mainHWND);
#ifdef D2_RENDER
            ValidateRect(hWnd, NULL);
#endif
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
