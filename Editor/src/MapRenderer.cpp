#include "pch.h"

#include <imgui.h>

#include "Editor/EditorTypes.h"
#include "Editor/Config/EditorConfiguration.h"
#include "Editor/MapRenderer.h"

namespace
{
    class EarClipper
    {
    public:
        struct Triangle { Core::Vector2 a, b, c; };

        static void Fill(ImDrawList* drawList, const std::vector<Core::Vector2>& points, ImU32 color)
        {
            std::vector<Triangle> triangles = Triangulate(points);

            ImVec2 uvWhite = ImGui::GetFontTexUvWhitePixel();
            int triCount = (int)triangles.size();
            drawList->PrimReserve(triCount * 3, triCount * 3);

            for (const Triangle& tri : triangles)
            {
                drawList->PrimVtx(tri.a, uvWhite, color);
                drawList->PrimVtx(tri.b, uvWhite, color);
                drawList->PrimVtx(tri.c, uvWhite, color);
            }
        }

    private:
        static float ComputeWindingSign(const std::vector<Core::Vector2>& points)
        {
            float area = 0.f;
            for (int i = 0, j = (int)points.size() - 1; i < (int)points.size(); j = i++)
                area += Core::Vector2::Cross(points[j], points[i]);

            return area > 0.f ? 1.f : -1.f;
        }

        static bool ClipNextEar(const std::vector<Core::Vector2>& points, std::vector<int>& idx, float windingSign, std::vector<Triangle>& triangles)
        {
            for (int i = 0; i < static_cast<int>(idx.size()); i++)
            {
                if (!IsEar(points, idx, i, windingSign)) continue;

                int n = static_cast<int>(idx.size());
                int prev = idx[(i + n - 1) % n], curr = idx[i], nxt = idx[(i + 1) % n];

                triangles.push_back({ points[prev], points[curr], points[nxt] });
                idx.erase(idx.begin() + i);
                return true;
            }
            return false;
        }

        static std::vector<Triangle> Triangulate(const std::vector<Core::Vector2>& points)
        {
            float windingSign = ComputeWindingSign(points);

            std::vector<int> idx(points.size());
            std::iota(idx.begin(), idx.end(), 0);

            std::vector<Triangle> triangles;
            triangles.reserve(points.size() - 2);

            while (idx.size() > 3)
                if (!ClipNextEar(points, idx, windingSign, triangles)) break;

            if (idx.size() == 3)
                triangles.push_back({ points[idx[0]], points[idx[1]], points[idx[2]] });

            return triangles;
        }

        static bool PointInTriangle(Core::Vector2 p, Core::Vector2 a, Core::Vector2 b, Core::Vector2 c)
        {
            float d1 = Core::Vector2::Cross(b - a, p - a);
            float d2 = Core::Vector2::Cross(c - b, p - b);
            float d3 = Core::Vector2::Cross(a - c, p - c);
            return !((d1 < 0 || d2 < 0 || d3 < 0) && (d1 > 0 || d2 > 0 || d3 > 0));
        }

        static bool IsEar(const std::vector<Core::Vector2>& pts, const std::vector<int>& idx, int i, float windingSign)
        {
            int n = (int)idx.size();
            Core::Vector2 a = pts[idx[(i + n - 1) % n]], b = pts[idx[i]], c = pts[idx[(i + 1) % n]];
            if (Core::Vector2::Cross(b - a, c - a) * windingSign <= 0.f) return false;
            for (int j = 0; j < n; j++)
            {
                if (j == (i + n - 1) % n || j == i || j == (i + 1) % n) continue;
                if (PointInTriangle(pts[idx[j]], a, b, c)) return false;
            }
            return true;
        }
    };
}

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
        if (sector.walls.size() < 2 || !IsInViewport(grid.WorldToScreen(sector.min), grid.WorldToScreen(sector.max))) return;

        std::vector<Core::Vector2> sectorPoints;
        GUID currentNode = sector.firstNodeID;

        for (const GUID& wallID : sector.walls)
        {
            const EditorWall& wall = data.GetWall(wallID);
            sectorPoints.push_back(grid.WorldToScreen(data.GetNode(currentNode).pos));
            currentNode = (wall.leftNodeID == currentNode) ? wall.rightNodeID : wall.leftNodeID;
        }

        if (sectorPoints.size() < 3) return;

        EarClipper::Fill(drawList, sectorPoints, ImGui::GetColorU32(GetSectorColor()));
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
