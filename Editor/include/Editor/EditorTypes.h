#pragma once

namespace Editor
{
    struct EditorWall
    {
        Core::Vector2 leftPoint, rightPoint;
        Core::Color topColor, innerColor, bottomColor;
        bool isPortal, isConnection, selected, hovered;
        uint64_t wallID = Core::NULL_ID_64, portalWallTargetID = Core::NULL_ID_64;
        uint32_t portalTargetSectorID = Core::NULL_ID_32;
    };

    struct EditorSector
    {
        uint32_t sectorID = Core::NULL_ID_32;
        float floorHeight = 0.f, ceillingHeight = 128.f;
        Core::Color floorColor, ceillingColor;
        std::vector<EditorWall> walls;
        bool selected, hovered;
    };

    Core::Sector EditorSectorToSector(const EditorSector& eSector);
    Core::Wall EditorWallToWall(const EditorWall& eWall, const uint32_t& editorSectorID);
}
