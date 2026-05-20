#pragma once

#include "Editor/MapRenderer.h"

namespace Editor
{
    class CommandHistory;
    class IEditorCommand;

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

        std::optional<std::reference_wrapper<EditorSector>> GetSelectedSector();
        std::optional<std::reference_wrapper<EditorWall>> GetSelectedWall();
        MapData& GetMapData() const { return *currentMapData; }

        float GetCurrentZoom() const { return grid->GetCurrentZoom(); }
        int GetCurrentGridSize() const { return grid->GetCurrentGridSize(); }
        void ToggleLineDrawMode(bool toggle) { isDrawingLine = toggle; }
        void SetCommandHistory(CommandHistory* history) { commandHistory = history; }
        void SyncAfterUndo(std::optional<std::reference_wrapper<const IEditorCommand>> cmd);
        void SyncAfterRedo(std::optional<std::reference_wrapper<const IEditorCommand>> cmd);

    private:
        void HandleInput();
        void HandleHotKeys();

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

        std::unique_ptr<Grid::MapGrid> grid;
        std::unique_ptr<MapData> currentMapData;
        std::unique_ptr<MapRenderer> mapRenderer;
        CommandHistory* commandHistory;

        bool firstRender, isDrawingLine, isHoveringWindow;
        float zoom = 1.f, cursorTime;
        int selectedSectorIndex = -1, selectedWallIndex = -1;
        std::optional<GUID> lineTargetNode, lastCreatedWallID;
        Core::Vector2 mapMaxSize;
    };
}
