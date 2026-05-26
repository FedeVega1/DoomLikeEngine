#include "pch.h"

#include <imgui.h>

#include "Editor/EditorTypes.h"
#include "Editor/Config/EditorConfiguration.h"
#include "Editor/MapRenderer.h"

namespace Editor
{
    MapRenderer::MapRenderer(const Grid::MapGrid& grid) : grid(grid) { }

    void MapRenderer::Render(const MapData& data)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        data.ForEachSector([&](const EditorSector& sector) { DrawSector(drawList, sector, data); });
        data.ForEachWall([&](const EditorWall& wall) { DrawWall(drawList, wall, data); });
        data.ForEachNode([&](const EditorNode& node) { DrawNode(drawList, node); });
    }

    bool MapRenderer::IsInViewport(const Core::Vector2& min, const Core::Vector2& max) const
    {
        Core::Vector2 windowMin = ImGui::GetWindowPos();
        Core::Vector2 windowMax = windowMin + Core::Vector2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
        return max.x >= windowMin.x && min.x <= windowMax.x && max.y >= windowMin.y && min.y <= windowMax.y;
    }

    ImVec4 MapRenderer::GetSectorColor() const
    {
        const DrawingSectorTheme& theme = ConfigurationManager::INS.GetSectorTheme();
        return theme.sectorColor;
    }

    ImVec4 MapRenderer::GetPortalColor(bool isConnection) const
    {
        const DrawingPortalTheme& theme = ConfigurationManager::INS.GetPortalTheme();
        if (isConnection) return theme.portalConnectionLine;
        return theme.portalLine;
    }

    ImVec4 MapRenderer::GetWallColor() const
    {
        const DrawingWallTheme& theme = ConfigurationManager::INS.GetWallTheme();
        return theme.wallLine;
    }

    float MapRenderer::GetWallThickness() const
    {
        const DrawingWallTheme& theme = ConfigurationManager::INS.GetWallTheme();
        return theme.wallLineThickness;
    }

    float MapRenderer::GetPortalThickness(bool isConnection) const
    {
        const DrawingPortalTheme& theme = ConfigurationManager::INS.GetPortalTheme();
        if (isConnection) return theme.portalConnectionLineThickness;
        return theme.portalLineThickness;
    }

    ImVec4 MapRenderer::GetNodeColor() const
    {
        const DrawingNodeTheme& theme = ConfigurationManager::INS.GetNodeTheme();
        return theme.nodePoint;
    }

    float MapRenderer::GetNodeThickness() const
    {
        const DrawingNodeTheme& theme = ConfigurationManager::INS.GetNodeTheme();
        return theme.nodePointThickness;
    }

    void MapRenderer::DrawSector(ImDrawList* drawList, const EditorSector& sector, const MapData& data)
    {
        if (sector.walls.empty() || !IsInViewport(grid.WorldToScreen(sector.min), grid.WorldToScreen(sector.max))) return;

        std::vector<ImVec2> sectorPoints;
        GUID currentNode = data.GetWall(sector.walls[0]).leftNodeID;

        for (const GUID& wallID : sector.walls)
        {
            const EditorWall& wall = data.GetWall(wallID);
            sectorPoints.push_back(grid.WorldToScreen(data.GetNode(currentNode).pos));
            currentNode = (wall.leftNodeID == currentNode) ? wall.rightNodeID : wall.leftNodeID;
        }

        drawList->AddConvexPolyFilled(sectorPoints.data(), sectorPoints.size(), ImGui::GetColorU32(GetSectorColor()));
    }

    void MapRenderer::DrawWall(ImDrawList* drawList, const EditorWall& wall, const MapData& data)
    {
        if (!IsInViewport(grid.WorldToScreen(wall.min), grid.WorldToScreen(wall.max))) return;

        const EditorNode& leftNode = data.GetNode(wall.leftNodeID);
        const EditorNode& rightNode = data.GetNode(wall.rightNodeID);

        ImVec4 color = wall.isPortal ? GetPortalColor(wall.isConnection) : GetWallColor();
        float thickness = wall.isPortal ? GetPortalThickness(wall.isConnection) : GetWallThickness();

        drawList->AddLine(grid.WorldToScreen(leftNode.pos), grid.WorldToScreen(rightNode.pos), ImGui::GetColorU32(color), thickness);
    }

    void MapRenderer::DrawNode(ImDrawList* drawList, const EditorNode& node)
    {
        // TODO: check selectionManager->IsSelected(node.nodeID, SelectableType::Node) for highlight color
        float thickness = GetNodeThickness();

        Core::Vector2 screenPos = grid.WorldToScreen(node.pos), offset = Core::Vector2::ONE * thickness;
        Core::Vector2 nodeMin = screenPos - offset, nodeMax = screenPos + offset;

        if (!IsInViewport(nodeMin, nodeMax)) return;

        drawList->AddRectFilled(nodeMin, nodeMax, ImGui::GetColorU32(GetNodeColor()));
    }
}
