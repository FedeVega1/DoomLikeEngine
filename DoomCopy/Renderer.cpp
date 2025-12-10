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

    Camera* const camera = game->GetMainCamera();
    camera->GetProcessedWalls(walls);

    for (size_t i = 0; i < walls.size(); i++) ProcessWall(walls[i], camera);
    std::swap(drawBuffer, screenBuffer);
}

void Renderer::ProcessWall(const ProcessedWall& wall, Camera* const camera)
{
    if (wall.leftBtmPoint.y < 1 && wall.rightBtmPoint.y < 1) return;

    if (wall.isPortal) { }

    Color pixelColor;
    ScreenSpaceWall sWall = GetScreenSpaceWall(wall);
    SpanResult res;

    BaseTexture wallText, ceillingText, floorText;
    GetTextureMap(L"test Wall.bmp", wallText);
    GetTextureMap(L"test Ceilling.bmp", ceillingText);
    GetTextureMap(L"test Floor.bmp", floorText);

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
        Vector2Int originalYPoint = yPoint;

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

        Vector2 screenSize = Vector2(static_cast<float>(DEFAULT_BUFFER_WIDTH), static_cast<float>(DEFAULT_BUFFER_HEIGHT));

        // Render floor with texture
        for (int y = floorStart; y < yPoint.x; y++) 
        {
            Vector2 screenCoords = Vector2(static_cast<float>(x), static_cast<float>(y));
            Vector2 normalizedScreenCoords = Vector2(screenCoords.x / screenSize.x, screenCoords.y / screenSize.y);

            Vector2 hitPoint = camera->GetFloorCeilingHitPoint(normalizedScreenCoords, wall.parentSector->bottomPoint);
            
            float distance = camera->GetDistanceToPoint(hitPoint) / brightnessDistanceFalloff;
            DrawPixel(x, y, DarkenPixelColor(floorText.MapFloorCeilingTexturePoint(hitPoint), distance));
        }

        // Render ceiling with texture
        for (int y = yPoint.y; y < ceillingEnd; y++) 
        {
            Vector2 screenCoords = Vector2(static_cast<float>(x), static_cast<float>(y));
            Vector2 normalizedScreenCoords = Vector2(screenCoords.x / screenSize.x, screenCoords.y / screenSize.y);

            Vector2 hitPoint = camera->GetFloorCeilingHitPoint(normalizedScreenCoords, wall.parentSector->topPoint);
            
            float distance = camera->GetDistanceToPoint(hitPoint) / brightnessDistanceFalloff;
            DrawPixel(x, y, DarkenPixelColor(ceillingText.MapFloorCeilingTexturePoint(hitPoint), distance));
        }

        if (wall.isConnection)
        {
            // If a connection is found, don't draw the wall and find it's neighbour wall.
            // Render the bottom and top walls from the difference in height between our
            // current sector and its neighbour
            InPortalRenderData renderData = InPortalRenderData { yPoint, diff, dX, x, camera, &wallText };
            OutPortalRenderData outData;
            RenderPortalWall(wall, renderData, outData);

            if (outData.hasDrawnC) currentSpan.ceilPoints[x] = outData.newCeillingY;
            if (outData.hasDrawnF) currentSpan.floorPoints[x] = outData.newFloorY;
        }
        else
        {
            Vector2 rayHit = camera->GetWorldPointFromRay(x, DEFAULT_BUFFER_WIDTH, wall);
            Vector2 dir = Vector2::Normalize(wall.referenceWall->rightPoint - wall.referenceWall->leftPoint);
            Vector2 offset = rayHit - wall.referenceWall->leftPoint;

            float dWall = offset.x * dir.x + offset.y * dir.y;
            float worldWallLength = Vector2::Distance(wall.referenceWall->leftPoint, wall.referenceWall->rightPoint);
            
            for (int y = yPoint.x; y < yPoint.y; y++)
            {
                int relativeY = originalYPoint.y - y - 1;
                float wallHeight = static_cast<float>(originalYPoint.y - originalYPoint.x);

                float distance = camera->GetDistanceToPoint(rayHit) / brightnessDistanceFalloff;
                DrawPixel(x, y, DarkenPixelColor(wallText.MapWallTexturePoint(dWall, relativeY, wallHeight, worldWallLength), distance));
            }
        }

        if (debugStepDraw) Sleep(1);
    }

    if (!drawn) return;
    spans.push_back(currentSpan);
}

Color Renderer::DarkenPixelColor(const Color& color, const float& value) const
{
    float brightness = std::clamp(1.0f - value, 0.0f, 1.0f);
    return Color(static_cast<BYTE>(color.r * brightness), static_cast<BYTE>(color.g * brightness), static_cast<BYTE>((color.b * brightness)));
}

ScreenSpaceWall Renderer::GetScreenSpaceWall(const ProcessedWall& wall)
{
    float fov = HALF_WIDTH;
    return ScreenSpaceWall
    {
        Vector2Int(static_cast<int>(std::roundf(((wall.leftTopPoint.x * fov) / wall.leftTopPoint.y) + HALF_WIDTH)), static_cast<int>(std::roundf(((wall.leftTopPoint.z * fov) / wall.leftTopPoint.y) + HALF_HEIGHT))),
        Vector2Int(static_cast<int>(std::roundf(((wall.rightTopPoint.x * fov) / wall.rightTopPoint.y) + HALF_WIDTH)),static_cast<int>(std::roundf(((wall.rightTopPoint.z * fov) / wall.rightTopPoint.y) + HALF_HEIGHT))),
        Vector2Int(static_cast<int>(std::roundf(((wall.leftBtmPoint.x * fov) / wall.leftBtmPoint.y) + HALF_WIDTH)), static_cast<int>(std::roundf(((wall.leftBtmPoint.z * fov) / wall.leftBtmPoint.y) + HALF_HEIGHT))),
        Vector2Int(static_cast<int>(std::roundf(((wall.rightBtmPoint.x * fov) / wall.rightBtmPoint.y) + HALF_WIDTH)), static_cast<int>(std::roundf(((wall.rightBtmPoint.z * fov) / wall.rightBtmPoint.y) + HALF_HEIGHT))),
        wall.topColor, wall.inColor, wall.btmColor,
    };
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

ProcessedWall* Renderer::GetProcessedWallPortalByID(const unsigned long long& wallID)
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

    ScreenSpaceWall sWall = GetScreenSpaceWall(*portalWall);
    int prevDYBtm = sWall.leftBtmPoint.y - sWall.rightBtmPoint.y;
    int prevDYTop = sWall.leftTopPoint.y - sWall.rightTopPoint.y;

    Vector2Int prevYPoint = Vector2Int(((prevDYBtm * data.diff) / data.dX) + sWall.rightBtmPoint.y, ((prevDYTop * data.diff) / data.dX) + sWall.rightTopPoint.y);
    Vector2Int originalPrevYPoint = prevYPoint;

    prevYPoint.x = std::clamp(prevYPoint.x, 0, DEFAULT_BUFFER_HEIGHT);
    prevYPoint.y = std::clamp(prevYPoint.y, 0, DEFAULT_BUFFER_HEIGHT);

    Vector2 rayHit = data.cameraRef->GetWorldPointFromRay(data.x, DEFAULT_BUFFER_WIDTH, *portalWall);
    Vector2 dir = Vector2::Normalize(portalWall->referenceWall->rightPoint - portalWall->referenceWall->leftPoint);
    Vector2 offset = rayHit - portalWall->referenceWall->leftPoint;

    float dWall = offset.x * dir.x + offset.y * dir.y;
    float worldWallLength = Vector2::Distance(portalWall->referenceWall->leftPoint, portalWall->referenceWall->rightPoint);

    if (wall.parentSector->bottomPoint < portalWall->parentSector->bottomPoint)
    {
        outData.newFloorY = prevYPoint.x;
        for (int y = data.yPoint.x; y < prevYPoint.x; y++)
        {
            int relativeY = originalPrevYPoint.y - y - 1;
            float wallHeight = static_cast<float>(originalPrevYPoint.y - originalPrevYPoint.x);

            float distance = data.cameraRef->GetDistanceToPoint(rayHit) / brightnessDistanceFalloff;
            DrawPixel(data.x, y, DarkenPixelColor(data.textureRef->MapWallTexturePoint(dWall, relativeY, wallHeight, worldWallLength), distance));
        }

        outData.hasDrawnF = true;
    }

    if (wall.parentSector->topPoint > portalWall->parentSector->topPoint)
    {
        outData.newCeillingY = prevYPoint.y;
        for (int y = prevYPoint.y; y < data.yPoint.y; y++)
        {
            int relativeY = originalPrevYPoint.y - y - 1;
            float wallHeight = static_cast<float>(originalPrevYPoint.y - originalPrevYPoint.x);

            float distance = data.cameraRef->GetDistanceToPoint(rayHit) / brightnessDistanceFalloff;
            DrawPixel(data.x, y, DarkenPixelColor(data.textureRef->MapWallTexturePoint(dWall, relativeY, wallHeight, worldWallLength), distance));
        }

        outData.hasDrawnC = true;
    }
}
