#include "pch.h"

#include <imgui.h>

#include "Editor/EditorTypes.h"
#include "Editor/MapView.h"

namespace Editor
{
    MapView::MapView()
    { 
        origin = Core::Vector2(0, 0);
        firstRender = isDrawingLine = isHoveringWindow = false;
        mapMaxSize = Core::Vector2(DEFAULT_MAX_MAP_SIZE_X, DEFAULT_MAX_MAP_SIZE_Y);
        grid = std::make_unique<Grid::MapGrid>(mapMaxSize);
    }

    void MapView::Update()
    {
        HandleInput();
    }

    void MapView::Render()
    {
        if (!firstRender)
        {
            grid->InitializeGrid();
            firstRender = true;
        }

        isHoveringWindow = ImGui::IsWindowHovered();

        DrawGrid();
        DrawSectors();
    }

    void MapView::NewMap()
    {
        sectors.clear();
        selectedSectorIndex = selectedWallIndex = -1;
        isDrawingLine = firstRender = false;
    }

    void MapView::LoadSectors(std::vector<EditorSector> loadedSectors)
    {
        sectors = std::move(loadedSectors);
        selectedSectorIndex = selectedWallIndex = -1;
    }

    EditorSector* MapView::GetSelectedSector()
    {
        if (selectedSectorIndex < 0 || selectedSectorIndex >= static_cast<int>(sectors.size())) return nullptr;
        return &sectors[selectedSectorIndex];
    }

    EditorWall* MapView::GetSelectedWall()
    {
        EditorSector* sector = GetSelectedSector();
        if (!sector) return nullptr;
        if (selectedWallIndex < 0 || selectedWallIndex >= static_cast<int>(sector->walls.size())) return nullptr;
        return &sector->walls[selectedWallIndex];
    }

    void MapView::HandleInput()
    {
        grid->HandleInputsNoFocus();
        if (!isHoveringWindow) return;
        grid->HandleInputs();
    }

    void MapView::DrawGrid()
    {
        grid->Render();
    }

    void MapView::DrawSectors()
    {
        for (const EditorSector& sector : sectors)
        {
            DrawSector(sector);
        }
    }

    void MapView::DrawSector(const EditorSector& sector)
    {
        for (const EditorWall& wall : sector.walls)
        {
            DrawWall(wall);
        }
    }

    void MapView::DrawWall(const EditorWall& wall)
    {
        // TODO: draw wall line using SDL render calls
    }
}
