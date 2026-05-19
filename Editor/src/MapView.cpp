#include "pch.h"

#include <imgui.h>

#include "Editor/EditorTypes.h"
#include "Editor/Config/EditorConfiguration.h"
#include "Editor/MapView.h"

namespace Editor
{
    MapView::MapView() : firstRender(false), isDrawingLine(false), isHoveringWindow(false), lineTargetNode(Core::NULL_ID_32),
        currentMapData(std::make_unique<MapData>(0UL, 0UL, 0UL)), lastCreatedWallID(Core::NULL_ID_32), cursorTime(0.f)
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
        DrawPreviewLine();
        DrawCursor();
    }

    void MapView::NewMap()
    {
        if (currentMapData)
        {
            currentMapData.release();
            currentMapData = nullptr;
        }

        currentMapData = std::make_unique<MapData>(0UL, 0UL, 0UL);

        selectedSectorIndex = selectedWallIndex = -1;
        lineTargetNode = lastCreatedWallID = Core::NULL_ID_32;
        isDrawingLine = firstRender = false;
    }

    void MapView::LoadSectors(std::vector<EditorSector> loadedSectors)
    {
        /*sectors = std::move(loadedSectors);*/
        currentMapData = std::make_unique<MapData>(0UL, 0UL, 0UL);
        lineTargetNode = lastCreatedWallID = Core::NULL_ID_32;
        selectedSectorIndex = selectedWallIndex = -1;
        isDrawingLine = firstRender = false;
    }

    EditorSector* MapView::GetSelectedSector()
    {
        //if (selectedSectorIndex < 0 || selectedSectorIndex >= static_cast<int>(currentMapData->sectors.size())) return nullptr;
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
        HandleHotKeys();

        if (!isDrawingLine || !ImGui::IsMouseClicked(ImGuiMouseButton_Left)) return;

        if (lineTargetNode != Core::NULL_ID_32)
        {
            GUID rightNodeID = currentMapData->AddNode(GetSnappedWorldPos());
            lastCreatedWallID = CreateWall(lineTargetNode, rightNodeID);
            lineTargetNode = rightNodeID;
            return;
        }

        lineTargetNode = currentMapData->AddNode(GetSnappedWorldPos());
    }

    void MapView::HandleHotKeys()
    {
        const MapEditorHotKeys& hotKeys = ConfigurationManager::INS.GetMapEditorHotKeys();
        if (!hotKeys.cancelAction.IsKeyPressed(false) || lineTargetNode == Core::NULL_ID_32) return;

        const EditorWall& lastCreatedWall = currentMapData->GetWall(lastCreatedWallID);
        if (lastCreatedWall.rightPoint != lineTargetNode) currentMapData->RemoveNode(lineTargetNode);
        lastCreatedWallID = lineTargetNode = Core::NULL_ID_32;
    }

    GUID MapView::CreateWall(GUID leftNodeID, GUID rightNodeID)
    {
        EditorWall wall = EditorWall
        {
            leftNodeID, rightNodeID,
            Core::Color::Black, Core::Color::Black, Core::Color::Black,
            false, false,
            Core::NULL_ID_32, Core::NULL_ID_32, Core::NULL_ID_32,
            Core::Vector2::ZERO, Core::Vector2::ZERO
        };

        EditorNode leftNode = currentMapData->GetNode(leftNodeID);

        wall.UpdateBounds(leftNode.pos, GetSnappedWorldPos());
        return currentMapData->AddWall(wall);
    }

    void MapView::DrawMapData()
    {
        if (!currentMapData) return;
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        currentMapData->ForEachSector([&](const EditorSector& sector) { DrawSector(drawList, sector); });
        currentMapData->ForEachWall([&](const EditorWall& wall) { DrawWall(drawList, wall); });
        currentMapData->ForEachNode([&](const EditorNode& node) { DrawNode(drawList, node); });
    }

    bool MapView::IsInViewport(const Core::Vector2& min, const Core::Vector2& max) const
    {
        Core::Vector2 windowMin = ImGui::GetWindowPos();
        Core::Vector2 windowMax = windowMin + Core::Vector2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        return max.x >= windowMin.x && min.x <= windowMax.x && max.y >= windowMin.y && min.y <= windowMax.y;
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
            const EditorWall& wall = currentMapData->GetWall(wallID);
            const EditorNode& node = currentMapData->GetNode(wall.leftPoint);
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

        const EditorNode& leftNode = currentMapData->GetNode(wall.leftPoint);
        const EditorNode& rightNode = currentMapData->GetNode(wall.rightPoint);

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

        Core::Vector2 screenPos = grid->WorldToScreen(node.pos), offset = Core::Vector2::ONE * thickness;
        Core::Vector2 nodeMin = screenPos - offset, nodeMax = screenPos + offset;

        if (!IsInViewport(nodeMin, nodeMax)) return;

        drawList->AddRectFilled(nodeMin, nodeMax, ImGui::GetColorU32(GetNodeColor()));
    }

    void MapView::DrawCursor()
    {
        if (!isHoveringWindow || !isDrawingLine) return;

        const EditorTheme& theme = ConfigurationManager::INS.GetEditorTheme();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        Core::Vector2 screenMousePos = GetScreenSpaceCursor();

        cursorTime += ImGui::GetIO().DeltaTime;
        float thickness = theme.cursorThickness;

        if (cursorTime >= 1.3f) cursorTime = 0.f;
        else if (cursorTime >= .65f) thickness *= .75f;

        Core::Vector2 offset = Core::Vector2::ONE * thickness;
        Core::Vector2 min = screenMousePos - offset, max = screenMousePos + offset;

        drawList->AddRectFilled(min, max, ImGui::GetColorU32(theme.cursorColor));
    }

    void MapView::DrawPreviewLine()
    {
        if (!isDrawingLine || lineTargetNode == Core::NULL_ID_32) return;

        const DrawingLineTheme& lineTheme = ConfigurationManager::INS.GetLineTheme();
        const DrawingNodeTheme& nodeTheme = ConfigurationManager::INS.GetNodeTheme();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const EditorNode& node = currentMapData->GetNode(lineTargetNode);

        Core::Vector2 screenNodePos = grid->WorldToScreen(node.pos);
        Core::Vector2 offset = Core::Vector2::ONE * nodeTheme.selectedNodePointThickness;
        Core::Vector2 min = screenNodePos - offset, max = screenNodePos + offset;

        drawList->AddRectFilled(min, max, ImGui::GetColorU32(nodeTheme.selectedNodePoint));

        drawList->AddLine(screenNodePos, GetScreenSpaceCursor(), ImGui::GetColorU32(lineTheme.drawingVectorLine),
            lineTheme.drawingLineThickness);
    }

    GUID MapData::AddNode(const Core::Vector2& pos)
    { 
        EditorNode newNode = { pos, ++nodeCounter };
        nodes.emplace(newNode.nodeID, newNode);
        return newNode.nodeID;
    }

    GUID MapData::AddWall(EditorWall& wall)
    { 
        wall.wallID = ++wallCounter; 
        walls.emplace(wall.wallID, wall); 
        return wall.wallID;
    }

    GUID MapData::AddSector(EditorSector& sector)
    { 
        sector.sectorID = ++sectorCounter; 
        sectors.emplace(sector.sectorID, sector); 
        return sector.sectorID;
    }

    void MapData::ForEachNode(const std::function<void(const EditorNode&)>& callback) const 
    { 
        for (const auto& [id, node] : nodes)
        {
            if (callback == nullptr) continue;
            callback(node);
        }
    }

    void MapData::ForEachWall(const std::function<void(const EditorWall&)>& callback) const
    {
        for (const auto& [id, wall] : walls)
        {
            if (callback == nullptr) continue;
            callback(wall);
        }
    }

    void MapData::ForEachSector(const std::function<void(const EditorSector&)>& callback) const
    {
        for (const auto& [id, sector] : sectors)
        {
            if (callback == nullptr) continue;
            callback(sector);
        }
    }
}
