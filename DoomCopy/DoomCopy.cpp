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
    SetupWindowMessages();

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
    GetClipCursor(&oldCursorClip);
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
    if (!messageMap.contains(message)) return DefWindowProc(hWnd, message, wParam, lParam);
    if (message == WM_COMMAND)
    {
        switch (LOWORD(wParam))
        {
            case IDM_EXIT: DestroyWindow(hWnd); return 0;
            default: return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    messageMap[message](wParam, lParam);
    return 0;
}

void SetupWindowMessages()
{
    messageMap = std::map<UINT, WMessagePointer>
    {
        { WM_KEYDOWN, [](WPARAM wParam, LPARAM lParam) { mainGame.CaptureKeyPress(wParam); if (wParam == VK_END) renderer.ToggleStepDraw(true); }},
        { WM_KEYUP, [](WPARAM wParam, LPARAM lParam) { mainGame.CaptureKeyRelease(wParam); } },
        { WM_LBUTTONUP, [](WPARAM wParam, LPARAM lParam) { mainGame.CaptureMouseRelease(0); } },
        { WM_RBUTTONDOWN, [](WPARAM wParam, LPARAM lParam) { mainGame.CaptureMousePress(1); } },
        { WM_RBUTTONUP, [](WPARAM wParam, LPARAM lParam) { mainGame.CaptureMouseRelease(1); } },
        { WM_MBUTTONDOWN, [](WPARAM wParam, LPARAM lParam) { mainGame.CaptureMousePress(2); } },
        { WM_MBUTTONUP, [](WPARAM wParam, LPARAM lParam) { mainGame.CaptureMouseRelease(2); } },
        { WM_XBUTTONDOWN, [](WPARAM wParam, LPARAM lParam) { mainGame.CaptureMousePress(GET_XBUTTON_WPARAM(wParam) == 1 ? 3 : 4); } },
        { WM_XBUTTONUP, [](WPARAM wParam, LPARAM lParam) { mainGame.CaptureMouseRelease(GET_XBUTTON_WPARAM(wParam) == 1 ? 3 : 4); } },

        { WM_MOUSEMOVE, [](WPARAM wPAram, LPARAM lParam) 
        { 
            POINTS mousePos = MAKEPOINTS(lParam);

            if (clippedCursor)
            {
                RECT r;
                GetClientRect(mainHWND, &r);
                bool overflow = false;

                if (mousePos.x >= r.right) { mousePos.x = (SHORT) r.left; overflow = true; }
                else if (mousePos.x <= r.left) { mousePos.x = (SHORT) r.right; overflow = true; }

                if (mousePos.y >= r.bottom) { mousePos.y = (SHORT) r.top; overflow = true; }
                else if (mousePos.y <= r.top) { mousePos.y = (SHORT) r.bottom; overflow = true; }

                if (overflow)
                {
                    POINT screenPos{ mousePos.x, mousePos.y };
                    ClientToScreen(mainHWND, &screenPos);
                    SetCursorPos(screenPos.x, screenPos.y);
                }
            }

            mainGame.CaptureMouseMovement(&mousePos, clippedCursor);
        } },

        { WM_CAPTURECHANGED, [](WPARAM wParam, LPARAM lParam) 
        { 
            ClipCursor(&oldCursorClip); 
            clippedCursor = false; 
        } },

        { WM_LBUTTONDOWN, [](WPARAM wParam, LPARAM lParam)
        {
            SetCapture(mainHWND);
            SetCursor(NULL);
            GetClipRect();
            ClipCursor(&newCursorClip);
            clippedCursor = true;
            mainGame.CaptureMousePress(0);
        } },

        { WM_DESTROY, [](WPARAM wParam, LPARAM lParam)
        {
            ClipCursor(&oldCursorClip);
            ReleaseCapture();
            clippedCursor = false;
            PostQuitMessage(0);
        } },

        { WM_PAINT, [](WPARAM wParam, LPARAM lParam)
        {
            renderer.RenderScreen(mainHWND, &mainGame);
#ifdef D2_RENDER
            ValidateRect(mainHWND, NULL);
#endif
        } },
    };
}

void GetClipRect()
{
    POINT upperLeft, lowerRight;

    GetClientRect(mainHWND, &newCursorClip);
    upperLeft.x = newCursorClip.left;
    upperLeft.y = newCursorClip.top;
    lowerRight.x = newCursorClip.right + 1;
    lowerRight.y = newCursorClip.bottom + 1;

    ClientToScreen(mainHWND, &upperLeft);
    ClientToScreen(mainHWND, &lowerRight);

    SetRect(&newCursorClip, upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);
}
