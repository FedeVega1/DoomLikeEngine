#include "framework.h"
#include "Renderer.h"

void Renderer::InitRenderer()
{
    buffer = new DWORD[DEFAULT_WIDTH * DEFAULT_HEIGHT]{0};
    PaintScreen(0x000000);
}

void Renderer::RenderScreen(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    for (int y = 0; y < DEFAULT_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_WIDTH; x++)
            SetPixel(hdc, x, y, buffer[x * (y + 1)]);
    }

    EndPaint(hwnd, &ps);
}

void Renderer::PaintScreen(DWORD color)
{
    for (int y = 0; y < DEFAULT_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_WIDTH; x++)
            buffer[x * (y + 1)] = color;
    }
}