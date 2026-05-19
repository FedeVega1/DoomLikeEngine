#pragma once

#include "Editor/MapGrid.h"

namespace Editor
{
    inline constexpr int DEFAULT_MAX_MAP_SIZE_X = 200;
    inline constexpr int DEFAULT_MAX_MAP_SIZE_Y = 200;

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
        void ToggleLineDrawMode(bool toggle) { isDrawingLine = toggle; }

    private:
        void HandleInput();
        void HandleHotKeys();

        void DrawMapData();
        void DrawSector(ImDrawList* const drawList, const EditorSector& sector);
        void DrawWall(ImDrawList* const drawList, const EditorWall& wall);
        void DrawNode(ImDrawList* const drawList, const EditorNode& node);

        void DrawCursor();
        void DrawPreviewLine();

        GUID CreateWall(GUID leftNodeID, GUID rightNodeID);

        Core::Vector2 GetSnappedWorldPos() const
        {
            Core::Vector2 worldPos = grid->ScreenToWorld(ImGui::GetMousePos());
            float gridSize = static_cast<float>(grid->GetCurrentGridSize());
            return Core::Vector2(std::round(worldPos.x / gridSize) * gridSize,
                                 std::round(worldPos.y / gridSize) * gridSize);
        }

        Core::Vector2 GetScreenSpaceCursor() const { return grid->WorldToScreen(GetSnappedWorldPos()); }
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
        float zoom = 1.f, cursorTime;
        int selectedSectorIndex = -1, selectedWallIndex = -1;
        GUID lineTargetNode, lastCreatedWallID;
        Core::Vector2 mapMaxSize;
    };
}
