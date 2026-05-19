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
        mapRenderer = std::make_unique<MapRenderer>(*grid);
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
        if (currentMapData) mapRenderer->Render(*currentMapData);
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
