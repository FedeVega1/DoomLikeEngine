#include "pch.h"

#include <imgui.h>

#include "Editor/EditorTypes.h"
#include "Editor/Config/EditorConfiguration.h"
#include "Editor/MapView.h"

namespace Editor
{
    MapView::MapView() : firstRender(false), isDrawingLine(false), isHoveringWindow(false), lineTargetNode(Core::NULL_ID_32)
    { 
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
            ImVec2 avail = ImGui::GetContentRegionAvail();
            if (avail.x > 1.f && avail.y > 1.f)
            {
                grid->InitializeGrid();
                firstRender = true;
            }
        }

        isHoveringWindow = ImGui::IsWindowHovered();

        grid->Render();
        DrawMapData();
    }

    void MapView::NewMap()
    {
        currentMapData.release();
        currentMapData = nullptr;

        selectedSectorIndex = selectedWallIndex = -1;
        isDrawingLine = firstRender = false;
    }

    void MapView::LoadSectors(std::vector<EditorSector> loadedSectors)
    {
        /*sectors = std::move(loadedSectors);*/
        selectedSectorIndex = selectedWallIndex = -1;
    }

    EditorSector* MapView::GetSelectedSector()
    {
        if (selectedSectorIndex < 0 || selectedSectorIndex >= static_cast<int>(currentMapData->sectors.size())) return nullptr;
        //return &sectors[selectedSectorIndex];
        return nullptr;
    }

    EditorWall* MapView::GetSelectedWall()
    {
        EditorSector* sector = GetSelectedSector();
        if (!sector) return nullptr;
        if (selectedWallIndex < 0 || selectedWallIndex >= static_cast<int>(sector->walls.size())) return nullptr;
        //return &sector->walls[selectedWallIndex];
        return nullptr;
    }

    void MapView::HandleInput()
    {
        grid->HandleInputsNoFocus();
        if (!isHoveringWindow) return;
        grid->HandleInputs();
    }

    void MapView::DrawMapData()
    {
        if (!currentMapData) return;
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (const auto& [id, sector] : currentMapData->sectors) DrawSector(drawList, sector);
        for (const auto& [id, wall] : currentMapData->walls) DrawWall(drawList, wall);
        for (const auto& [id, node] : currentMapData->nodes) DrawNode(drawList, node);
    }

    bool MapView::IsInViewport(const Core::Vector2& min, const Core::Vector2& max) const
    {
        Core::Vector2 windowMin = ImGui::GetWindowPos();
        Core::Vector2 windowMax = windowMin + Core::Vector2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        return max.x >= windowMin.x && min.x <= windowMax.x &&
               max.y >= windowMin.y && min.y <= windowMax.y;
    }

    ImVec4 MapView::GetSectorColor() const
    {
        const DrawingSectorTheme& theme = ConfigurationManager::INS.GetSectorTheme();
        return theme.sectorColor;
    }

    void MapView::DrawSector(ImDrawList* const drawList, const EditorSector& sector)
    {
        if (!IsInViewport(grid->WorldToScreen(sector.min), grid->WorldToScreen(sector.max))) return;

        std::vector<ImVec2> sectorPoints;
        for (const GUID& wallID : sector.walls)
        {
            const EditorWall& wall = currentMapData->walls.at(wallID);
            const EditorNode& node = currentMapData->nodes.at(wall.leftPoint);
            sectorPoints.push_back(grid->WorldToScreen(node.pos));
        }

        drawList->AddConvexPolyFilled(sectorPoints.data(), sectorPoints.size(), ImGui::GetColorU32(GetSectorColor()));
    }

    ImVec4 MapView::GetPortalColor(bool isConnection) const
    {
        const DrawingPortalTheme& theme = ConfigurationManager::INS.GetPortalTheme();
        if (isConnection) return theme.portalConnectionLine;
        return theme.portalLine;
    }

    ImVec4 MapView::GetWallColor() const
    {
        const DrawingWallTheme& theme = ConfigurationManager::INS.GetWallTheme();
        return theme.wallLine;
    }

    float MapView::GetWallThickness() const
    {
        const DrawingWallTheme& theme = ConfigurationManager::INS.GetWallTheme();
        return theme.wallLineThickness;
    }

    float MapView::GetPortalThickness(bool isConnection) const
    {
        const DrawingPortalTheme& theme = ConfigurationManager::INS.GetPortalTheme();
        if (isConnection) return theme.portalConnectionLineThickness;
        return theme.portalLineThickness;
    }

    void MapView::DrawWall(ImDrawList* const drawList, const EditorWall& wall)
    {
        if (!IsInViewport(grid->WorldToScreen(wall.min), grid->WorldToScreen(wall.max))) return;

        const EditorNode& leftNode = currentMapData->nodes.at(wall.leftPoint);
        const EditorNode& rightNode = currentMapData->nodes.at(wall.rightPoint);

        ImVec4 color = wall.isPortal ? GetPortalColor(wall.isConnection) : GetWallColor();
        float thickness = wall.isPortal ? GetPortalThickness(wall.isConnection) : GetWallThickness();

        drawList->AddLine(grid->WorldToScreen(leftNode.pos), grid->WorldToScreen(rightNode.pos), ImGui::GetColorU32(color), thickness);
    }

    ImVec4 MapView::GetNodeColor() const
    {
        const DrawingNodeTheme& theme = ConfigurationManager::INS.GetNodeTheme();
        return theme.nodePoint;
    }

    float MapView::GetNodeThickness() const
    {
        const DrawingNodeTheme& theme = ConfigurationManager::INS.GetNodeTheme();
        return theme.nodePointThickness;
    }

    void MapView::DrawNode(ImDrawList* const drawList, const EditorNode& node)
    {
        float thickness = GetNodeThickness();
        Core::Vector2 screenPos = grid->WorldToScreen(node.pos);
        Core::Vector2 nodeMin = screenPos - Core::Vector2::ONE * thickness;
        Core::Vector2 nodeMax = screenPos + Core::Vector2::ONE * thickness;

        if (!IsInViewport(nodeMin, nodeMax)) return;

        drawList->AddRectFilled(nodeMin, nodeMax, ImGui::GetColorU32(GetNodeColor()));
    }
}
