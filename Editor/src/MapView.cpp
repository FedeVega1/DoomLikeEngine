#include "pch.h"

#include <imgui.h>
#include <SDL3/SDL.h>

#include "Editor/EditorTypes.h"
#include "Editor/MapView.h"

namespace Editor
{
    MapView::MapView(SDL_Renderer* renderer) : renderer(renderer) {}

    void MapView::Update()
    {
        HandleInput();
    }

    void MapView::Render()
    {
        DrawGrid();
        DrawSectors();
    }

    void MapView::NewMap()
    {
        sectors.clear();
        selectedSectorIndex = selectedWallIndex = -1;
        isDrawingLine = false;
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
        // TODO: implement pan, zoom, wall drawing, selection
    }

    void MapView::DrawGrid()
    {
        // TODO: draw world grid using SDL draw calls within an ImGui child window
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
