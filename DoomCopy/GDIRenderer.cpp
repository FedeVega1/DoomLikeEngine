#include "pch.h"
#include "Game.h"
#include "Renderer.h"
#include "GameObjects.h"
#include "VectorMath.h"
#include "CameraComponent.h"
#include "World.h"
#include "GDIRenderer.h"

HRESULT GDIRenderer::InitRenderer(HWND hwnd)
{
    buffer = new DWORD[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT] {0};
    //buffer = std::make_shared<DWORD>(DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT);

    walls = std::vector<ProcessedWall>();

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

void GDIRenderer::RenderScreen(HWND hwnd, Game* const game)
{
    if (bitmapInfo.bmiHeader.biSize == 0) return;

    PAINTSTRUCT ps;
    debugHDC = BeginPaint(hwnd, &ps);

    // DEBUG
    PaintScreen(Color(0, 0, 0));
    
    game->GetMainCamera()->GetProcessedWalls(walls);

    //for (int x = -5; x < 6; x++)
    //{
    //    for (int y = -5; y < 6; y++)
    //        DrawPixel(x + HALF_WIDTH, y + HALF_HEIGHT, COLOR_RED);
    //}

    //DrawPixel(HALF_WIDTH, HALF_HEIGHT, COLOR_GREEN);

    //for (size_t i = 0; i < walls.size(); i++)
    //{
    //    ScreenSpaceWall swall = ScreenSpaceWall
    //    {
    //        V2INT_ZERO, V2INT_ZERO,
    //        Vector2Int((int) std::roundf(((walls[i].leftBtmPoint.x) / walls[i].rightBtmPoint.y) + HALF_WIDTH), (int) std::roundf(((walls[i].leftBtmPoint.z) / walls[i].leftBtmPoint.y) + HALF_HEIGHT)),
    //        Vector2Int((int) std::roundf(((walls[i].rightBtmPoint.x) / walls[i].rightBtmPoint.y) + HALF_WIDTH), (int) std::roundf(((walls[i].rightBtmPoint.z) / walls[i].rightBtmPoint.y) + HALF_HEIGHT)),
    //        walls[i].topColor, walls[i].inColor, walls[i].btmColor,
    //    };

    //    for (int x = walls[i].leftBtmPoint.x; x < walls[i].rightBtmPoint.x; x++)
    //    {
    //        for (int y = -5; y < 6; y++)
    //        {

    //        }
    //    }

    //    DrawPixel(HALF_WIDTH, HALF_HEIGHT, COLOR_GREEN);
    //}

    for (size_t i = 0; i < walls.size(); i++) ProcessWall(walls[i]);
    // DEBUG

    int scanlines = StretchDIBits(debugHDC, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT, buffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    if (scanlines == 0) OLOG_E("Failed to Draw Bitmap!");

    if (debugStepDraw)
    {
        Sleep(500);
        ToggleStepDraw(false);
    }

    EndPaint(hwnd, &ps);
}

void GDIRenderer::PaintScreen(Color color)
{
    for (int y = 0; y < DEFAULT_BUFFER_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_BUFFER_WIDTH; x++)
            buffer[PixelPos(x, y)] = color.ToDWORD(true);
    }
}

void GDIRenderer::DrawPixel(int x, int y, Color color)
{
    x = std::clamp(x, 0, DEFAULT_BUFFER_WIDTH - 1);
    y = std::clamp(y, 0, DEFAULT_BUFFER_HEIGHT - 1);

    buffer[PixelPos(x, y)] = color.ToDWORD(true);

    if (debugStepDraw)
    {
        int scanlines = StretchDIBits(debugHDC, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT, buffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
        if (scanlines == 0) OLOG_E("Failed to Draw Bitmap!");
        //Sleep(1);
    }
}

GDIRenderer::GDIRenderer() : buffer(nullptr), bitmapInfo(), hbmp(nullptr), memHDC(nullptr), debugHDC(nullptr)
{ }

GDIRenderer::~GDIRenderer()
{
    DeleteObject(hbmp);
    delete[] buffer;
}
