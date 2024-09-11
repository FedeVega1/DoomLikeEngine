#include "framework.h"
#include "Renderer.h"

void Renderer::InitRenderer()
{
    buffer = std::shared_ptr<DWORD>(new DWORD[DEFAULT_WIDTH * DEFAULT_HEIGHT] {0});
    PaintScreen(0x000000);
}

void Renderer::RenderScreen(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

#ifdef GDI_SETPIXEL
    for (int y = 0; y < DEFAULT_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_WIDTH; x++)
            SetPixel(hdc, x, y, buffer.get()[x * (y + 1)]);
    }
#else
    BITMAPINFOHEADER header = {
        sizeof(BITMAPINFOHEADER),
        DEFAULT_WIDTH,
        DEFAULT_HEIGHT,
        1,
        32,
        BI_RGB,
        0,
        10,
        10
    };

    BITMAPINFO bitmapInfo = { header };

    StretchDIBits(hdc, 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT, buffer.get(), &bitmapInfo, 0, SRCCOPY);
#endif // GDI_SETPIXEL

    EndPaint(hwnd, &ps);
}

void Renderer::PaintScreen(DWORD color)
{
    for (int y = 0; y < DEFAULT_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_WIDTH; x++)
            buffer.get()[PIXELPOS(x, y)] = color;
    }
}

void Renderer::SetPixel(int x, int y, DWORD color)
{
    buffer.get()[PIXELPOS(x, y)] = color;
}
