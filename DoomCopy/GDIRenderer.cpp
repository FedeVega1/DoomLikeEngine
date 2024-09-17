#include "framework.h"
#include "Game.h"
#include "Renderer.h"
#include "GDIRenderer.h"
#include "TransformComponent.h"

HRESULT GDIRenderer::InitRenderer(HWND hwnd)
{
    buffer = new DWORD[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT] {0};
    //buffer = std::make_shared<DWORD>(DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT);

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

    StretchDIBits(hdc, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT, buffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    EndPaint(hwnd, &ps);
}

void GDIRenderer::PaintScreen(DWORD color)
{
    for (int y = 0; y < DEFAULT_BUFFER_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_BUFFER_WIDTH; x++)
            buffer[PixelPos(x, y)] = color;
    }
}

void GDIRenderer::DrawPixel(int x, int y, DWORD color)
{
    if (x > DEFAULT_BUFFER_WIDTH) x = DEFAULT_BUFFER_WIDTH;
    else if (x < 0) x = 0;

    if (y > DEFAULT_BUFFER_HEIGHT) y = DEFAULT_BUFFER_HEIGHT;
    else if (y < 0) y = 0;

    buffer[PixelPos(x, y)] = color;
}

void GDIRenderer::ProcessGame(HWND hwnd, std::shared_ptr<Game> game) 
{
    PaintScreen(0);

    size_t count = game->GetEntityCount();
    for (size_t i = 0; i < count; i++)
    {
        GameObject* go = game->GetGameObject(i);
        if (!go) continue;
        Vector3 pos = go->GetTransform()->GetPos();
        DrawPixel(std::roundf(pos.x), std::roundf(pos.y), 0xFF0000);
    }

    InvalidateRect(hwnd, NULL, false);
}

GDIRenderer::GDIRenderer() : buffer(nullptr), bitmapInfo(), hbmp(nullptr), memHDC(nullptr)
{ }

GDIRenderer::~GDIRenderer()
{
    DeleteObject(hbmp);
    delete[] buffer;
}
