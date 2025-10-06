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
    spans.clear();

    game->GetMainCamera()->GetProcessedWalls(walls);
    for (size_t i = 0; i < walls.size(); i++) ProcessWall(walls[i]);
    std::swap(drawBuffer, screenBuffer);
}

void Renderer::ProcessWall(const ProcessedWall& wall)
{
    if (wall.leftBtmPoint.y < 1 && wall.rightBtmPoint.y < 1) return;

    if (wall.isPortal) { }

    ScreenSpaceWall sWall = GetScreenSpaceWall(wall);
    SpanResult res;

    if (IsWallOccluded(sWall.GetSegment(), res)) return;
    ScreenSpan currentSpan = ScreenSpan(res.segment, wall.isConnection);
    bool behindConn = res.connectionIndx != -1;

    // Grab the ΔY & ΔX corresponding to the top and bottom lines of the wall
    int dYBtm = sWall.rightBtmPoint.y - sWall.leftBtmPoint.y;
    int dYTop = sWall.rightTopPoint.y - sWall.leftTopPoint.y;
    int dX = sWall.rightBtmPoint.x - sWall.leftBtmPoint.x;
    if (dX == 0) dX = 1; // Avoid division by 0

    int startX = sWall.leftTopPoint.x;

    sWall.leftTopPoint.x = std::clamp(res.segment.x, 0, DEFAULT_BUFFER_WIDTH);
    sWall.rightTopPoint.x = std::clamp(res.segment.y, 0, DEFAULT_BUFFER_WIDTH);

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

        int floorStart = 0, ceillingEnd = DEFAULT_BUFFER_HEIGHT;
        if (behindConn)
        {
            floorStart = spans[res.connectionIndx].floorPoints[x];
            ceillingEnd = spans[res.connectionIndx].ceilPoints[x];
        }

        currentSpan.floorPoints[x] = yPoint.x;
        currentSpan.ceilPoints[x] = yPoint.y;

        for (int y = floorStart; y < yPoint.x; y++) DrawPixel(x, y, wall.parentSector->floorColor);
        for (int y = yPoint.y; y < ceillingEnd; y++) DrawPixel(x, y, wall.parentSector->ceillingColor);

        if (wall.isConnection)
        {
            // If a connection is found, don't draw the wall and find it's neighbour wall.
            // Render the bottom and top walls from the difference in height between our
            // current sector and its neighbour
            InPortalRenderData renderData = InPortalRenderData { yPoint, diff, dX, x };
            OutPortalRenderData outData;
            RenderPortalWall(wall, renderData, outData);

            if (outData.hasDrawnC) currentSpan.ceilPoints[x] = outData.newCeillingY;
            if (outData.hasDrawnF) currentSpan.floorPoints[x] = outData.newFloorY;
        }
        else
        {
            // Render the actual wall
            for (int y = yPoint.x; y < yPoint.y; y++)
                DrawPixel(x, y, sWall.inColor);
        }

        if (debugStepDraw) Sleep(1);
    }

    if (!drawn) return;
    spans.push_back(currentSpan);
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

bool Renderer::IsWallOccluded(Vector2Int wallSegment, SpanResult& result)
{
    result.segment = wallSegment;
    for (size_t i = 0; i < spans.size(); i++)
    {
        if (!spans[i].Intersects(wallSegment)) continue;

        if (spans[i].isConnection)
        {
            result.connectionIndx = i;
            continue;
        }

        if (spans[i].ClampToSpan(wallSegment))
            return IsWallOccluded(wallSegment, result);

        return true;
    }

    return false;
}

bool ScreenSpan::Intersects(Vector2Int otherSegment) const
{
    return std::max(otherSegment.x, spanSegment.x) < std::min(otherSegment.y, spanSegment.y);
}

bool ScreenSpan::ClampToSpan(Vector2Int& outOtherSegment) const
{
    int x = outOtherSegment.x, y = outOtherSegment.y;
    bool clamped = false;

    if (y > spanSegment.x && x < spanSegment.x) { y = spanSegment.x; clamped = true; }
    if (x < spanSegment.y && y > spanSegment.y) { x = spanSegment.y; clamped = true; }
    outOtherSegment = Vector2Int(x, y);
    return clamped;
}

ProcessedWall* Renderer::GetProcessedWallPortalByID(unsigned long long wallID)
{
    for (size_t i = 0; i < walls.size(); i++)
    {
        if (walls[i].referenceWall->wallID == wallID)
            return &walls[i];
    }

    return nullptr;
}

void Renderer::RenderPortalWall(const ProcessedWall& wall, const InPortalRenderData& data, OutPortalRenderData& outData)
{
    if (!wall.portalTargetWall) return;
    ProcessedWall* portalWall = GetProcessedWallPortalByID(wall.portalTargetWall->wallID);
    if (!portalWall) return;

    if (wall.parentSector->bottomPoint < portalWall->parentSector->bottomPoint)
    {
        ScreenSpaceWall sWall = GetScreenSpaceWall(*portalWall);
        int prevDYBtm = sWall.leftBtmPoint.y - sWall.rightBtmPoint.y;
        int prevYPoint = ((prevDYBtm * data.diff) / data.dX) + sWall.rightBtmPoint.y;
        outData.newFloorY = prevYPoint;
        for (int y = data.yPoint.x; y < prevYPoint; y++) DrawPixel(data.x, y, wall.btmColor);
        outData.hasDrawnF = true;
    }

    if (wall.parentSector->topPoint > portalWall->parentSector->topPoint)
    {
        ScreenSpaceWall sWall = GetScreenSpaceWall(*portalWall);
        int prevDYTop = sWall.leftTopPoint.y - sWall.rightTopPoint.y;
        int prevYPoint = ((prevDYTop * data.diff) / data.dX) + sWall.rightTopPoint.y;
        outData.newCeillingY = prevYPoint;
        for (int y = prevYPoint; y < data.yPoint.y; y++) DrawPixel(data.x, y, wall.topColor);
        outData.hasDrawnC = true;
    }
}
