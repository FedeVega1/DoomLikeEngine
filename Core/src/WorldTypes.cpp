#include "pch.h"

#include "Core/VectorMath.h"
#include "Core/WorldTypes.h"

namespace Core
{
    bool Wall::VectorInFront(const Vector2& point) const
    {
        Vector2 seg = { rightPoint.x - leftPoint.x, rightPoint.y - leftPoint.y };
        Vector2 toPoint = { point.x - leftPoint.x, point.y - leftPoint.y };
        return Vector2::Cross(seg, toPoint) > 0;
    }

    bool Splitter::VectorInFront(const Vector2& point) const
    {
        Vector2 seg = { static_cast<float>(segment.x), static_cast<float>(segment.y) };
        Vector2 toPoint = { point.x - startPoint.x, point.y - startPoint.y };
        return Vector2::Cross(seg, toPoint) > 0;
    }

    void BSPMap::LinkPortals()
    {
        for (auto& subSector : subSectors)
        {
            for (auto& wall : subSector.walls)
            {
                if (wall.portalWallTargetID == NULL_ID_64) continue;
                wall.portalTargetWall = FindWallByID(wall.portalWallTargetID);
            }
        }
    }

    Wall* BSPMap::FindWallByID(uint64_t wallID)
    {
        for (auto& subSector : subSectors)
        {
            for (auto& wall : subSector.walls)
            {
                if (wall.wallID == wallID) return &wall;
            }
        }
        return nullptr;
    }

    const Sector* BSPMap::FindSectorByID(uint32_t sectorID) const
    {
        for (const auto& sector : sectors)
        {
            if (sector.sectorID == sectorID) return &sector;
        }
        return nullptr;
    }
}
