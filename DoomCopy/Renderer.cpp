#include "framework.h"
#include "Renderer.h"

void Renderer::InitRenderer(HWND hwnd)
{
    HDC hDesktop = GetDC(hwnd);
    memHDC = CreateCompatibleDC(hDesktop);
    hbmp = CreateCompatibleBitmap(hDesktop, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    SelectObject(memHDC, hbmp);

    //buffer = std::shared_ptr<DWORD>(new DWORD[DEFAULT_WIDTH * DEFAULT_HEIGHT] {0});
    PaintScreen(0x000000);
}

void Renderer::RenderScreen(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    //for (int y = 0; y < DEFAULT_HEIGHT; y++)
    //{
    //    for (int x = 0; x < DEFAULT_WIDTH; x++)
    //        SetPixel(memHDC, x, y, buffer.get()[PIXELPOS(x, y)]);
    //}

    BitBlt(hdc, 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT, memHDC, 0, 0, SRCCOPY);

    EndPaint(hwnd, &ps);
}

void Renderer::PaintScreen(DWORD color)
{
    for (int y = 0; y < DEFAULT_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_WIDTH; x++)
            SetPixel(memHDC, x, y, color);
    }
}

void Renderer::DrawPixel(int x, int y, DWORD color)
{
    SetPixel(memHDC, x, y, color);
}

Renderer::~Renderer()
{
    DeleteObject(hbmp);
}