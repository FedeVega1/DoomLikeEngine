#pragma once

namespace Editor
{
    struct EditorNode
    {
        Core::Vector2 pos;
        GUID nodeID = Core::NULL_ID_32;
    };

    struct EditorWall
    {
        GUID leftPoint = Core::NULL_ID_32, rightPoint = Core::NULL_ID_32;
        Core::Color topColor, innerColor, bottomColor;
        bool isPortal = false, isConnection = false;
        GUID wallID = Core::NULL_ID_32, portalWallTargetID = Core::NULL_ID_32;
        GUID portalTargetSectorID = Core::NULL_ID_32;
        Core::Vector2 min, max;

        void UpdateBounds(const Core::Vector2& a, const Core::Vector2& b)
        {
            min = Core::Vector2(std::min(a.x, b.x), std::min(a.y, b.y));
            max = Core::Vector2(std::max(a.x, b.x), std::max(a.y, b.y));
        }
    };

    struct EditorSector
    {
        GUID sectorID = Core::NULL_ID_32;
        float floorHeight = 0.f, ceillingHeight = 128.f;
        Core::Color floorColor, ceillingColor;
        std::vector<GUID> walls;
        Core::Vector2 min, max;

        void ResetBounds()
        {
            min = Core::Vector2(FLT_MAX, FLT_MAX);
            max = Core::Vector2(-FLT_MAX, -FLT_MAX);
        }

        void ExpandBounds(const Core::Vector2& point)
        {
            min = Core::Vector2(std::min(min.x, point.x), std::min(min.y, point.y));
            max = Core::Vector2(std::max(max.x, point.x), std::max(max.y, point.y));
        }
    };

    struct MapData
    {
        std::unordered_map<GUID, EditorNode> nodes;
        std::unordered_map<GUID, EditorWall> walls;
        std::unordered_map<GUID, EditorSector> sectors;
    };

    Core::Sector EditorSectorToSector(const EditorSector& eSector);
    Core::Wall EditorWallToWall(const EditorWall& eWall, const GUID& editorSectorID);
}
