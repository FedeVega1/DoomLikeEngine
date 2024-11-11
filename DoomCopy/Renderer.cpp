#include "pch.h"
#include "Game.h"
#include "GameObjects.h"
#include "VectorMath.h"
#include "CameraComponent.h"
#include "Renderer.h"

void Renderer::ProcessGame(HWND hwnd, Game* const game)
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

void Renderer::ProcessSector(const ProcessedSector& sector)
{
    int size = sector.numberOfWalls * 2;
    int surfacePoints[2][DEFAULT_BUFFER_WIDTH];

    for (int s = 0; s < DEFAULT_BUFFER_WIDTH; s++)
    {
        surfacePoints[0][s] = 0;
        surfacePoints[1][s] = DEFAULT_BUFFER_HEIGHT;
    }

    for (int i = 0; i < size; i++)
    {
        if (sector.sectorWalls[i].leftBtmPoint.y < 1 && sector.sectorWalls[i].rightBtmPoint.y < 1) continue;

        ScreenSpaceWall sWall = GetScreenSpaceWall(sector.sectorWalls[i]);
        int dYBtm = sWall.rightBtmPoint.y - sWall.leftBtmPoint.y;
        int dYTop = sWall.rightTopPoint.y - sWall.leftTopPoint.y;
        int dX = sWall.rightBtmPoint.x - sWall.leftBtmPoint.x;
        if (dX == 0) dX = 1;
        int startX = sWall.leftTopPoint.x;

        sWall.leftTopPoint.x = std::clamp(sWall.leftTopPoint.x, 0, DEFAULT_BUFFER_WIDTH);
        sWall.rightTopPoint.x = std::clamp(sWall.rightTopPoint.x, 0, DEFAULT_BUFFER_WIDTH);

        for (int x = sWall.leftTopPoint.x; x < sWall.rightTopPoint.x; x++)
        {
            int diff = x - startX + .5f;
            Vector2Int yPoint = Vector2Int(((dYBtm * diff) / dX) + sWall.leftBtmPoint.y, ((dYTop * diff) / dX) + sWall.leftTopPoint.y);

            yPoint.x = std::clamp(yPoint.x, 0, DEFAULT_BUFFER_HEIGHT);
            yPoint.y = std::clamp(yPoint.y, 0, DEFAULT_BUFFER_HEIGHT);

            if (i % 2 == 0)
            {
                surfacePoints[0][x] = yPoint.y;
                surfacePoints[1][x] = yPoint.x;
                continue;
            }

            for (int y = surfacePoints[0][x]; y < yPoint.x; y++) DrawPixel(x, y, sector.floorColor);
            for (int y = yPoint.y; y < surfacePoints[1][x]; y++) DrawPixel(x, y, sector.ceillingColor);
            for (int y = yPoint.x; y < yPoint.y; y++) DrawPixel(x, y, sector.sectorWalls[i].color);
            if (debugStepDraw) Sleep(50);
        }
    }
}

ScreenSpaceWall Renderer::GetScreenSpaceWall(const ProcessedWall& wall)
{
    float fov = 200;
    return ScreenSpaceWall
    {
        Vector2Int((int) std::roundf(((wall.leftTopPoint.x * fov) / wall.leftTopPoint.y) + HALF_WIDTH), (int) std::roundf(((wall.leftTopPoint.z * fov) / wall.leftTopPoint.y) + HALF_HEIGHT)),
        Vector2Int((int) std::roundf(((wall.rightTopPoint.x * fov) / wall.rightTopPoint.y) + HALF_WIDTH), (int) std::roundf(((wall.rightTopPoint.z * fov) / wall.rightTopPoint.y) + HALF_HEIGHT)),
        Vector2Int((int) std::roundf(((wall.leftBtmPoint.x * fov) / wall.leftBtmPoint.y) + HALF_WIDTH), (int) std::roundf(((wall.leftBtmPoint.z * fov) / wall.leftBtmPoint.y) + HALF_HEIGHT)),
        Vector2Int((int) std::roundf(((wall.rightBtmPoint.x * fov) / wall.rightBtmPoint.y) + HALF_WIDTH), (int) std::roundf(((wall.rightBtmPoint.z * fov) / wall.rightBtmPoint.y) + HALF_HEIGHT)),
        wall.color
    };
}
