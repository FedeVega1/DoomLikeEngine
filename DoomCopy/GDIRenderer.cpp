#include "framework.h"
#include "Renderer.h"
#include "GDIRenderer.h"

HRESULT GDIRenderer::InitRenderer(HWND hwnd)
{
    buffer = std::shared_ptr<DWORD>(new DWORD[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT] {0});

    HDC hDesktop = GetDC(hwnd);
    memHDC = CreateCompatibleDC(hDesktop);
    hbmp = CreateCompatibleBitmap(hDesktop, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT);

    bitmapInfo = BITMAPINFO { 
        BITMAPINFOHEADER 
        { 
            sizeof(BITMAPINFOHEADER),
            DEFAULT_BUFFER_WIDTH,
            -DEFAULT_BUFFER_HEIGHT,
            1,
            32,
            BI_RGB,
            DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT
        } 
    };

    SelectObject(memHDC, hbmp);
    return S_OK;
}

void GDIRenderer::RenderScreen(HWND hwnd)
{
    if (bitmapInfo.bmiHeader.biSize == 0) return;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    StretchDIBits(hdc, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT, buffer.get(), &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    EndPaint(hwnd, &ps);
}

void GDIRenderer::PaintScreen(DWORD color)
{
    for (int y = 0; y < DEFAULT_BUFFER_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_BUFFER_WIDTH; x++)
            buffer.get()[PixelPos(x, y)] = color;
    }
}

void GDIRenderer::DrawPixel(int x, int y, DWORD color)
{
    if (x > DEFAULT_BUFFER_WIDTH) x = DEFAULT_BUFFER_WIDTH;
    else if (x < 0) x = 0;

    if (y > DEFAULT_BUFFER_HEIGHT) y = DEFAULT_BUFFER_HEIGHT;
    else if (y < 0) y = 0;

    int pos = PixelPos(x, y);
    //OLOG_LF("Pos {0}", pos);
    buffer.get()[pos] = color;
}

GDIRenderer::~GDIRenderer()
{
    DeleteObject(hbmp);
}