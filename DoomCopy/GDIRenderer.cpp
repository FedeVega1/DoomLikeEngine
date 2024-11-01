#include "pch.h"
#include "Game.h"
#include "Renderer.h"
#include "GameObjects.h"
#include "VectorMath.h"
#include "CameraComponent.h"
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

void GDIRenderer::RenderScreen(HWND hwnd, Game* const game)
{
    if (bitmapInfo.bmiHeader.biSize == 0) return;

    PAINTSTRUCT ps;
    debugHDC = BeginPaint(hwnd, &ps);

    // DEBUG
    PaintScreen(Color(0, 0, 0));

    const ProcessedSector* sectors = nullptr;
    int numbSectors = game->GetMainCamera()->GetProcessedSectors(&sectors);
    if (!sectors) return;
    for (int i = 0; i < numbSectors; i++) ProcessSector(sectors[i]);
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

void GDIRenderer::ProcessGame(HWND hwnd, Game* const game) 
{
    //PaintScreen(Color(0, 0, 0));

    //size_t count = game->GetEntityCount();
    //for (size_t i = 0; i < count; i++)
    //{
    //    GameObject* go = game->GetGameObject(i);
    //    if (!go) continue;
    //    Vector3 pos = go->GetTransform()->GetPos();
    //    DrawPixel((int) std::roundf(pos.x), (int) std::roundf(pos.y), 0xFF0000);
    //}
    //const ProcessedSector* sectors = nullptr;
    //int numbSectors = game->GetMainCamera()->GetProcessedSectors(&sectors);
    //if (!sectors) return;
    //for (int i = 0; i < numbSectors; i++) ProcessSector(sectors[i]);

    InvalidateRect(hwnd, NULL, false);
}

GDIRenderer::GDIRenderer() : buffer(nullptr), bitmapInfo(), hbmp(nullptr), memHDC(nullptr), debugStepDraw(false), debugHDC(nullptr)
{ }

GDIRenderer::~GDIRenderer()
{
    DeleteObject(hbmp);
    delete[] buffer;
}

void GDIRenderer::ProcessSector(const ProcessedSector& sector) 
{
    int size = sector.numberOfWalls;
    std::array<int, DEFAULT_BUFFER_WIDTH>* surfacePoints = new std::array<int, DEFAULT_BUFFER_WIDTH>[size];

    for (int i = 0; i < size; i++)
    {
        switch (sector.surface)
        {
            case SectorSurface::Above:
                for (int c = 0; c < DEFAULT_BUFFER_WIDTH; c++)
                    surfacePoints[i][c] = 0;
                break;

            case SectorSurface::Below:
                for (int c = 0; c < DEFAULT_BUFFER_WIDTH; c++)
                    surfacePoints[i][c] = DEFAULT_BUFFER_HEIGHT;
                break;
        }

        int indx = (i * 2) + 1;
        DrawBackWall(sector.sectorWalls[indx], sector.surface, surfacePoints[i]);
        //DrawSurfaces(sector.sectorWalls[indx], sector.surface, surfacePoints[i], sector.ceillingColor, sector.floorColor);
    }

    for (int i = 0; i < size; i++)
        DrawWall(sector.sectorWalls[i * 2], sector.surface);

    delete[] surfacePoints;
}

void GDIRenderer::DrawWall(const ProcessedWall& wall, SectorSurface surface)
{
    if (wall.leftBtmPoint.y < 1 && wall.rightBtmPoint.y < 1) return;

    ScreenSpaceWall sWall = GetScreenSpaceWall(wall);
    int dyBottom = sWall.rightBtmPoint.y - sWall.leftBtmPoint.y;
    int dyTop = sWall.rightTopPoint.y - sWall.leftTopPoint.y;
    int dx = sWall.rightBtmPoint.x - sWall.leftBtmPoint.x;
    if (dx == 0) dx = 1;
    int xStartPoint = sWall.leftBtmPoint.x;

    sWall.leftBtmPoint.x = std::clamp(sWall.leftBtmPoint.x, 0, DEFAULT_BUFFER_WIDTH);
    sWall.rightBtmPoint.x = std::clamp(sWall.rightBtmPoint.x, 0, DEFAULT_BUFFER_WIDTH);

    bool debugDraw = false;
    for (int x = sWall.leftBtmPoint.x; x < sWall.rightBtmPoint.x; x++)
    {
        debugDraw = true;
        int diff = x - xStartPoint + .5f;
        int yBtm = ((dyBottom * diff) / dx) + sWall.leftBtmPoint.y;
        int yTop = ((dyTop * diff) / dx) + sWall.leftTopPoint.y;

        yBtm = std::clamp(yBtm, 0, DEFAULT_BUFFER_HEIGHT);
        yTop = std::clamp(yTop, 0, DEFAULT_BUFFER_HEIGHT);

        for (int y = yBtm; y < yTop; y++) DrawPixel(x, y, sWall.color);
        if (debugStepDraw) Sleep(50);
    }

    if (debugStepDraw && debugDraw) OLOG_L("Wall");
}

void GDIRenderer::DrawBackWall(const ProcessedWall& wall, SectorSurface surface, std::array<int, DEFAULT_BUFFER_WIDTH>& points)
{
    if (wall.leftBtmPoint.y < 1 && wall.rightBtmPoint.y < 1) return;

    ScreenSpaceWall sWall = GetScreenSpaceWall(wall);
    int dyBottom = sWall.rightBtmPoint.y - sWall.leftBtmPoint.y;
    int dyTop = sWall.rightTopPoint.y - sWall.leftTopPoint.y;
    int dx = sWall.rightBtmPoint.x - sWall.leftBtmPoint.x;
    if (dx == 0) dx = 1;
    int xStartPoint = sWall.leftBtmPoint.x;

    sWall.leftBtmPoint.x = std::clamp(sWall.leftBtmPoint.x, 0, DEFAULT_BUFFER_WIDTH);
    sWall.rightBtmPoint.x = std::clamp(sWall.rightBtmPoint.x, 0, DEFAULT_BUFFER_WIDTH);

    bool debugDraw = false;
    for (int x = sWall.leftBtmPoint.x; x < sWall.rightBtmPoint.x; x++)
    {
        debugDraw = true;
        int diff = x - xStartPoint + .5f;
        int yBtm = ((dyBottom * diff) / dx) + sWall.leftBtmPoint.y;
        int yTop = ((dyTop * diff) / dx) + sWall.leftTopPoint.y;

        yBtm = std::clamp(yBtm, 0, DEFAULT_BUFFER_HEIGHT);
        yTop = std::clamp(yTop, 0, DEFAULT_BUFFER_HEIGHT);

        switch (surface)
        {
            case SectorSurface::Above: points[x] = yBtm; break;
            case SectorSurface::Below: points[x] = yTop; break;
        }

        for (int y = yBtm; y < yTop; y++) DrawPixel(x, y, sWall.color);
        if (debugStepDraw) Sleep(50);
    }

    if (debugStepDraw && debugDraw) OLOG_L("BackWall");
}

void GDIRenderer::DrawSurfaces(const ProcessedWall& wall, SectorSurface surface, const std::array<int, DEFAULT_BUFFER_WIDTH>& points, Color ceilling, Color floor)
{
    if (wall.leftBtmPoint.y < 1 && wall.rightBtmPoint.y < 1) return;

    ScreenSpaceWall sWall = GetScreenSpaceWall(wall);
    int dyBottom = sWall.rightBtmPoint.y - sWall.leftBtmPoint.y;
    int dyTop = sWall.rightTopPoint.y - sWall.leftTopPoint.y;
    int dx = sWall.rightBtmPoint.x - sWall.leftBtmPoint.x;
    if (dx == 0) dx = 1;
    int xStartPoint = sWall.leftBtmPoint.x;

    sWall.leftBtmPoint.x = std::clamp(sWall.leftBtmPoint.x, 0, DEFAULT_BUFFER_WIDTH);
    sWall.rightBtmPoint.x = std::clamp(sWall.rightBtmPoint.x, 0, DEFAULT_BUFFER_WIDTH);

    bool debugDraw = false;
    for (int x = sWall.leftBtmPoint.x; x < sWall.rightBtmPoint.x; x++)
    {
        debugDraw = true;
        int diff = x - xStartPoint + .5f;
        int yBtm = ((dyBottom * diff) / dx) + sWall.leftBtmPoint.y;
        int yTop = ((dyTop * diff) / dx) + sWall.leftTopPoint.y;

        yBtm = std::clamp(yBtm, 0, DEFAULT_BUFFER_HEIGHT);
        yTop = std::clamp(yTop, 0, DEFAULT_BUFFER_HEIGHT);

        switch (surface)
        {
            case SectorSurface::Above:
                for (int y = points[x]; y < yTop; y++)
                    DrawPixel(x, y, ceilling);
                break;

            case SectorSurface::Below:
                for (int y = yBtm; y < points[x]; y++)
                    DrawPixel(x, y, floor);
                break;
        }

        if (debugStepDraw) Sleep(50);
    }

    if (debugStepDraw && debugDraw) OLOG_L("Surface");
}

ScreenSpaceWall GDIRenderer::GetScreenSpaceWall(const ProcessedWall& wall)
{
    return ScreenSpaceWall
    {
        Vector2Int((int) std::roundf(((wall.leftTopPoint.x * 90) / wall.leftTopPoint.y) + HALF_WIDTH), (int) std::roundf(((wall.leftTopPoint.z * 90) / wall.leftTopPoint.y) + HALF_HEIGHT)),
        Vector2Int((int) std::roundf(((wall.rightTopPoint.x * 90) / wall.rightTopPoint.y) + HALF_WIDTH), (int) std::roundf(((wall.rightTopPoint.z  * 90) / wall.rightTopPoint.y) + HALF_HEIGHT)),
        Vector2Int((int) std::roundf(((wall.leftBtmPoint.x * 90) / wall.leftBtmPoint.y) + HALF_WIDTH), (int) std::roundf(((wall.leftBtmPoint.z * 90) / wall.leftBtmPoint.y) + HALF_HEIGHT)),
        Vector2Int((int) std::roundf(((wall.rightBtmPoint.x * 90) / wall.rightBtmPoint.y) + HALF_WIDTH), (int) std::roundf(((wall.rightBtmPoint.z * 90) / wall.rightBtmPoint.y) + HALF_HEIGHT)),
        wall.color
    };
}
