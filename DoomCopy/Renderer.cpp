#include "pch.h"
#include "Game.h"
#include "GameObjects.h"
#include "VectorMath.h"
#include "CameraComponent.h"
#include "World.h"
#include "Renderer.h"

void Renderer::ProcessGame(Game* const game)
{
    PaintScreen(Color(0, 0, 0));
    segments.clear();

    game->GetMainCamera()->GetProcessedWalls(walls);
    for (size_t i = 0; i < walls.size(); i++) ProcessWall(walls[i]);
    std::swap(drawBuffer, screenBuffer);
}

void Renderer::ProcessWall(const ProcessedWall& wall)
{
    if (wall.leftBtmPoint.y < 1 && wall.rightBtmPoint.y < 1) return;

    if (wall.isPortal) { }

    ScreenSpaceWall sWall = GetScreenSpaceWall(wall);
    if (IsWallOccluded(Vector2Int(sWall.leftBtmPoint.x, sWall.rightBtmPoint.x))) return;

    // Grab the ΔY & ΔX corresponding to the top and bottom lines of the wall
    int dYBtm = sWall.rightBtmPoint.y - sWall.leftBtmPoint.y;
    int dYTop = sWall.rightTopPoint.y - sWall.leftTopPoint.y;
    int dX = sWall.rightBtmPoint.x - sWall.leftBtmPoint.x;
    if (dX == 0) dX = 1; // Avoid division by 0

    int startX = sWall.leftTopPoint.x;

    sWall.leftTopPoint.x = std::clamp(sWall.leftTopPoint.x, 0, DEFAULT_BUFFER_WIDTH);
    sWall.rightTopPoint.x = std::clamp(sWall.rightTopPoint.x, 0, DEFAULT_BUFFER_WIDTH);

    bool drawn = false;
    for (int x = sWall.leftTopPoint.x; x < sWall.rightTopPoint.x; x++)
    {
        drawn = true;
        // Get the Y position of each pixel across their ΔY

        int diff = x - startX + .5f;
        Vector2Int yPoint = Vector2Int(((dYBtm * diff) / dX) + sWall.leftBtmPoint.y, ((dYTop * diff) / dX) + sWall.leftTopPoint.y);

        yPoint.x = std::clamp(yPoint.x, 0, DEFAULT_BUFFER_HEIGHT);
        yPoint.y = std::clamp(yPoint.y, 0, DEFAULT_BUFFER_HEIGHT);

        // Draw the Ceilling and Floor from their respective line into the Screen limits

        for (int y = 0; y < yPoint.x; y++) DrawPixel(x, y, wall.parentSector->floorColor);
        for (int y = yPoint.y; y < DEFAULT_BUFFER_HEIGHT; y++) DrawPixel(x, y, wall.parentSector->ceillingColor);

        if (wall.isConnection)
        {
            drawn = false;
            // If a connection is found, don't draw the wall and find it's neighbour wall.
            // Render the bottom and top walls from the difference in height between our
            // current sector and its neighbour

            //int sectIndx = GetSectorIndexFromID(wall.portalTargetSector, wallPtr, numbWalls);
            //if (sectIndx == -1) continue;
            //const ProcessedSector& prevSector =  sectorPtr[sectIndx];

            //if (sector.bottomPoint < prevSector.bottomPoint)
            //{
            //    ScreenSpaceWall prevSWall = GetScreenSpaceWall(prevSector.sectorWalls[sector.sectorWalls[i].portalTargetWall]);
            //    int prevDYBtm = prevSWall.leftBtmPoint.y - prevSWall.rightBtmPoint.y;
            //    int prevYPoint = ((prevDYBtm * diff) / dX) + prevSWall.rightBtmPoint.y;
            //    for (int y = yPoint.x; y < prevYPoint; y++) DrawPixel(x, y, sWall.btmColor);
            //}

            //if (sector.topPoint > prevSector.topPoint)
            //{
            //    ScreenSpaceWall prevSWall = GetScreenSpaceWall(prevSector.sectorWalls[sector.sectorWalls[i].portalTargetWall]);
            //    int prevDYTop = prevSWall.leftTopPoint.y - prevSWall.rightTopPoint.y;
            //    int prevYPoint = ((prevDYTop * diff) / dX) + prevSWall.rightTopPoint.y;
            //    for (int y = prevYPoint; y < yPoint.y; y++) DrawPixel(x, y, sWall.topColor);
            //}
        }
        else
        {
            // Render the actual wall
            for (int y = yPoint.x; y < yPoint.y; y++)
                DrawPixel(x, y, sWall.inColor);
        }

        if (debugStepDraw) Sleep(50);
    }

    if (!drawn) return;
    AddNewSegment(Vector2Int(sWall.leftBtmPoint.x, sWall.rightBtmPoint.x));
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
        wall.topColor, wall.inColor, wall.btmColor,
    };
}

Vector3 Renderer::GetWallNormal(Vector3 pointA, Vector3 pointB) 
{
    Vector3 dir1 = Vector3::Normalize(pointB - pointA);
    Vector3 dir2 = Vector3::Normalize(pointA - pointB);
    return Vector3::Cross(dir1, dir2);
}

bool Renderer::IsWallOccluded(Vector2Int wallSegment)
{
    for (size_t i = 0; i < segments.size(); i++)
    {
        if (wallSegment.x <= segments[i].x || wallSegment.y >= segments[i].y) continue;
        return true;
    }

    return false;
}

void Renderer::AddNewSegment(Vector2Int newSegment)
{
    for (size_t i = 0; i < segments.size(); i++)
    {
        if (std::abs(segments[i].y - newSegment.x) < kEpsilon)
        {
            newSegment = Vector2Int(segments[i].x, newSegment.y);
            segments.erase(segments.begin() + i);
            AddNewSegment(newSegment);
            return;
        }

        if (std::abs(segments[i].x - newSegment.y) < kEpsilon)
        {
            newSegment = Vector2Int(newSegment.x, segments[i].y);
            segments.erase(segments.begin() + i);
            AddNewSegment(newSegment);
            return;
        }
    }

    segments.push_back(newSegment);
}
