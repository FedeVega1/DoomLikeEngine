#pragma once

#include "Editor/MapGrid.h"

namespace Editor
{
    class MapRenderer
    {
    public:
        explicit MapRenderer(const Grid::MapGrid& grid);
        void Render(const MapData& data);

    private:
        void DrawSector(ImDrawList* drawList, const EditorSector& sector, const MapData& data);
        void DrawWall(ImDrawList* drawList, const EditorWall& wall, const MapData& data);
        void DrawNode(ImDrawList* drawList, const EditorNode& node);

        bool IsInViewport(const Core::Vector2& min, const Core::Vector2& max) const;

        ImVec4 GetSectorColor() const;
        ImVec4 GetPortalColor(bool isConnection) const;
        ImVec4 GetWallColor() const;
        ImVec4 GetNodeColor() const;

        float GetPortalThickness(bool isConnection) const;
        float GetWallThickness() const;
        float GetNodeThickness() const;

        const Grid::MapGrid& grid;
    };
}
