#pragma once

#include "Editor/MapGrid.h"

namespace Editor
{
    inline constexpr int DEFAULT_MAX_MAP_SIZE_X = 50;
    inline constexpr int DEFAULT_MAX_MAP_SIZE_Y = 50;

    class MapView
    {
    public:
        MapView();

        void Update();
        void Render();

        void NewMap();
        void LoadSectors(std::vector<EditorSector> sectors);
        const std::vector<EditorSector>& GetSectors() const { return {}; }

        EditorSector* GetSelectedSector();
        EditorWall* GetSelectedWall();

        float GetCurrentZoom() const { return grid->GetCurrentZoom(); }
        int GetCurrentGridSize() const { return grid->GetCurrentGridSize(); }

    private:
        void HandleInput();
        void DrawMapData();
        void DrawSector(ImDrawList* const drawList, const EditorSector& sector);
        void DrawWall(ImDrawList* const drawList, const EditorWall& wall);
        void DrawNode(ImDrawList* const drawList, const EditorNode& node);

        bool IsInViewport(const Core::Vector2& min, const Core::Vector2& max) const;

        ImVec4 GetSectorColor() const;
        ImVec4 GetPortalColor(bool isConnection) const;
        ImVec4 GetWallColor() const;
        ImVec4 GetNodeColor() const;

        float GetPortalThickness(bool isConnection) const;
        float GetWallThickness() const;
        float GetNodeThickness() const;

        std::unique_ptr<Grid::MapGrid> grid;
        std::unique_ptr<MapData> currentMapData;

        bool firstRender, isDrawingLine, isHoveringWindow;
        float zoom = 1.f;
        int selectedSectorIndex = -1, selectedWallIndex = -1;
        GUID lineTargetNode;
        Core::Vector2 mapMaxSize, lineStart, linePreview;
    };
}
