#include "pch.h"
#include "Game.h"
#include "Renderer.h"
#include "GameObjects.h"
#include "VectorMath.h"
#include "World.h"
#include "GDIRenderer.h"

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

    int scanlines = StretchDIBits(hdc, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT, buffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    if (scanlines == 0) OLOG_E("Failed to Draw Bitmap!");
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
    if (x >= DEFAULT_BUFFER_WIDTH) x = DEFAULT_BUFFER_WIDTH - 1;
    else if (x < 0) x = 0;

    if (y >= DEFAULT_BUFFER_HEIGHT) y = DEFAULT_BUFFER_HEIGHT - 1;
    else if (y < 0) y = 0;

    buffer[PixelPos(x, y)] = color;
}

void GDIRenderer::ProcessGame(HWND hwnd, std::shared_ptr<Game> game) 
{
    PaintScreen(0);

    //size_t count = game->GetEntityCount();
    //for (size_t i = 0; i < count; i++)
    //{
    //    GameObject* go = game->GetGameObject(i);
    //    if (!go) continue;
    //    Vector3 pos = go->GetTransform()->GetPos();
    //    DrawPixel((int) std::roundf(pos.x), (int) std::roundf(pos.y), 0xFF0000);
    //}

    DrawWall(game->GetWorldRef()->wall);

    //if (screenPosA.x > 0 && screenPosA.x < DEFAULT_BUFFER_WIDTH && screenPosA.y > 0 && screenPosA.y < DEFAULT_BUFFER_HEIGHT) DrawPixel((int)std::roundf(screenPosA.x), (int)std::roundf(screenPosA.y), 0xFF0000);
    //if (screenPosB.x > 0 && screenPosB.x < DEFAULT_BUFFER_WIDTH && screenPosB.y > 0 && screenPosB.y < DEFAULT_BUFFER_HEIGHT) DrawPixel((int)std::roundf(screenPosB.x), (int)std::roundf(screenPosB.y), 0xFF0000);

    InvalidateRect(hwnd, NULL, false);
}

GDIRenderer::GDIRenderer() : buffer(nullptr), bitmapInfo(), hbmp(nullptr), memHDC(nullptr)
{ }

GDIRenderer::~GDIRenderer()
{
    DeleteObject(hbmp);
    delete[] buffer;
}

void GDIRenderer::DrawWall(const Wall& wall)
{
    if (wall.leftBtmPoint.z < 1 && wall.rightBtmPoint.z < 1) return;

    ScreenSpaceWall sWall = GetScreenSpaceWall(wall);
    int dyb = sWall.leftBtmPoint.y - sWall.rightBtmPoint.y;
    int dyt = sWall.rightTopPoint.y - sWall.leftTopPoint.y;
    int dx = sWall.leftBtmPoint.x - sWall.rightBtmPoint.x;
    if (dx == 0) dx = 1;
    int xs = sWall.leftBtmPoint.x;

    if (sWall.leftBtmPoint.x < 0) sWall.leftBtmPoint.x = 0;
    if (sWall.rightBtmPoint.x < 0) sWall.rightBtmPoint.x = 0;

    if (sWall.leftBtmPoint.x > DEFAULT_BUFFER_WIDTH) sWall.leftBtmPoint.x = DEFAULT_BUFFER_WIDTH;
    if (sWall.rightBtmPoint.x > DEFAULT_BUFFER_WIDTH) sWall.rightBtmPoint.x = DEFAULT_BUFFER_WIDTH;

    for (int x = sWall.leftBtmPoint.x; x < sWall.rightBtmPoint.x; x++)
    {
        int y1 = dyb * (x - xs + .5) / dx + sWall.leftBtmPoint.y;
        int y2 = dyt * (x - xs + .5) / dx + sWall.leftTopPoint.y;

        if (y1 < 0) y1 = 0;
        if (y2 < 0) y2 = 0;

        if (y1 > DEFAULT_BUFFER_HEIGHT) y1 = DEFAULT_BUFFER_HEIGHT;
        if (y2 > DEFAULT_BUFFER_HEIGHT) y2 = DEFAULT_BUFFER_HEIGHT;

        for (int y = y1; y < y2; y++) DrawPixel(x, y, 0xFF0000);
    }
}

ScreenSpaceWall GDIRenderer::GetScreenSpaceWall(const Wall& wall)
{
    return ScreenSpaceWall
    {
        Vector2Int((int) std::roundf(wall.leftTopPoint.x * 200 / wall.leftTopPoint.z + HALF_WIDTH), (int) std::roundf(wall.leftTopPoint.y * 200 / wall.leftTopPoint.z + HALF_HEIGHT)),
        Vector2Int((int) std::roundf(wall.rightTopPoint.x * 200 / wall.rightTopPoint.z + HALF_WIDTH), (int) std::roundf(wall.rightTopPoint.y * 200 / wall.rightTopPoint.z + HALF_HEIGHT)),
        Vector2Int((int) std::roundf(wall.leftBtmPoint.x * 200 / wall.leftBtmPoint.z + HALF_WIDTH), (int) std::roundf(wall.leftBtmPoint.y * 200 / wall.leftBtmPoint.z + HALF_HEIGHT)),
        Vector2Int((int) std::roundf(wall.rightBtmPoint.x * 200 / wall.rightBtmPoint.z + HALF_WIDTH), (int) std::roundf(wall.rightBtmPoint.y * 200 / wall.rightBtmPoint.z + HALF_HEIGHT))
    };
}
