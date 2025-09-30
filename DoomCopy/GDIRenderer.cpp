#include "pch.h"
#include "Game.h"
#include "Renderer.h"
#include "GameObjects.h"
#include "VectorMath.h"
#include "CameraComponent.h"
#include "World.h"
#include "GDIRenderer.h"

HRESULT GDIRenderer::InitRenderer(HWND _hwnd)
{
    screenBuffer = new DWORD[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT] {0};
    drawBuffer = new DWORD[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT] {0};

    walls = std::vector<ProcessedWall>();
    hwnd = _hwnd;

    HDC hDesktop = GetDC(hwnd);
    memHDC = CreateCompatibleDC(hDesktop);
    hbmp = CreateCompatibleBitmap(hDesktop, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT);

    bitmapInfo = BITMAPINFO { 
        BITMAPINFOHEADER 
        { 
            sizeof(BITMAPINFOHEADER),
            DEFAULT_BUFFER_WIDTH,
            DEFAULT_BUFFER_HEIGHT,
            1,
            32,
            BI_RGB,
            DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT
        } 
    };

    SelectObject(memHDC, hbmp);
    return S_OK;
}

void GDIRenderer::RenderScreen()
{
    if (bitmapInfo.bmiHeader.biSize == 0) return;

    HDC hdc = GetDC(hwnd);

    int scanlines = StretchDIBits(hdc, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT, screenBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    if (scanlines == 0) OLOG_E("Failed to Draw Bitmap!");

    if (debugStepDraw)
    {
        Sleep(1);
        ToggleStepDraw(false);
    }

    ReleaseDC(hwnd, hdc);
}

void GDIRenderer::PaintScreen(Color color)
{
    for (int y = 0; y < DEFAULT_BUFFER_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_BUFFER_WIDTH; x++)
            drawBuffer[PixelPos(x, y)] = color.ToDWORD(true);
    }
}

void GDIRenderer::DrawPixel(int x, int y, Color color)
{
    x = std::clamp(x, 0, DEFAULT_BUFFER_WIDTH - 1);
    y = std::clamp(y, 0, DEFAULT_BUFFER_HEIGHT - 1);

    drawBuffer[PixelPos(x, y)] = color.ToDWORD(true);

    if (debugStepDraw)
    {
        HDC hdc = GetDC(hwnd);
        int scanlines = StretchDIBits(hdc, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT, drawBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
        if (scanlines == 0) OLOG_E("DEBUG - Failed to Draw Bitmap!");
        //Sleep(1);
        ReleaseDC(hwnd, hdc);
    }
}

GDIRenderer::GDIRenderer() : bitmapInfo(), hbmp(nullptr), memHDC(nullptr)
{ }

GDIRenderer::~GDIRenderer()
{
    DeleteObject(hbmp);
    delete[] screenBuffer;
    delete[] drawBuffer;
}
