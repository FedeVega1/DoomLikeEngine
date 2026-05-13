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
        const std::vector<EditorSector>& GetSectors() const { return sectors; }

        EditorSector* GetSelectedSector();
        EditorWall* GetSelectedWall();

        float GetCurrentZoom() const { return grid->GetCurrentZoom(); }
        int GetCurrentGridSize() const { return grid->GetCurrentGridSize(); }

    private:
        void HandleInput();
        void DrawGrid();
        void DrawSectors();
        void DrawSector(const EditorSector& sector);
        void DrawWall(const EditorWall& wall);

        std::unique_ptr<Grid::MapGrid> grid;

        std::vector<EditorSector> sectors;

        bool firstRender, isDrawingLine, isHoveringWindow;
        float zoom = 1.f;
        int gridUnitSize = 20, selectedSectorIndex = -1, selectedWallIndex = -1;
        Core::Vector2 origin, mapMaxSize, lineStart, linePreview;
    };
}
