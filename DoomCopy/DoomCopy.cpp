#include "framework.h"
#include <chrono>
#include "DoomCopy.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef GDI_RENDER
    renderer = GDIRenderer();
#endif

#ifdef D2_RENDER
    renderer = D2Renderer();
#endif

    InitLogSystem(true, false);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DOOMCOPY, szWindowClass, MAX_LOADSTRING);

    if (!SetupAndCreateWindow(hInstance, nCmdShow)) return -1;
    if (IS_ERROR(renderer.InitRenderer(mainHWND))) { Sleep(5000); return -1; }
    return MainLoop();
}

BOOL SetupAndCreateWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEXW wcex = {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DOOMCOPY)),
        LoadCursor(nullptr, IDC_ARROW),
        NULL,
        NULL,
        szWindowClass,
        LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL))
    };

    startTime = std::chrono::high_resolution_clock::now();
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
    
    long long nextGameTick = GetGameTickCount();
    int loops = 0;

    int startingPixel = 100, movePixel = startingPixel;
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        loops = 0;

        long long start = GetGameTickCount();
        while (GetGameTickCount() > nextGameTick && loops < MAX_FRAMESKIP)
        {
            // GameLoop
            movePixel++;
            if ((movePixel - startingPixel) > 100) movePixel = startingPixel;
            OLOG_LF("{0} | {1}", movePixel, loops);

            nextGameTick += SKIP_TICKS;
            loops++;
        }

        renderer.PaintScreen(0);

        for (int i = 0; i < 250; i++)
            renderer.DrawPixel(55 + i, movePixel, 0xFF0000);

        renderer.DrawPixel(0, 0, 0);
        InvalidateRect(mainHWND, NULL, false);
    }

    return (int) msg.wParam;
}

long long GetGameTickCount()
{
    std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
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
