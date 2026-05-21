#include "pch.h"

#include <imgui.h>

#include "Editor/EditorTypes.h"
#include "Editor/Config/EditorConfiguration.h"
#include "Editor/CommandHistory.h"
#include "Editor/EditorCommands.h"
#include "Editor/SelectionManager.h"
#include "Editor/MapView.h"

namespace Editor
{
    MapView::MapView() : firstRender(false), isDrawingLine(false), isHoveringWindow(false),
        currentMapData(std::make_unique<MapData>(0UL, 0UL, 0UL)), cursorTime(0.f),
        commandHistory(nullptr)
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

        lineTargetNode = lastCreatedWallID = std::nullopt;
        isDrawingLine = firstRender = false;
    }

    void MapView::LoadSectors(std::vector<EditorSector> loadedSectors)
    {
        /*sectors = std::move(loadedSectors);*/
        currentMapData = std::make_unique<MapData>(0UL, 0UL, 0UL);
        lineTargetNode = lastCreatedWallID = std::nullopt;
        isDrawingLine = firstRender = false;
    }

    void MapView::HandleInput()
    {
        grid->HandleInputsNoFocus();
        if (!isHoveringWindow) return;
        grid->HandleInputs();
        HandleHotKeys();

        if (!isDrawingLine || !ImGui::IsMouseClicked(ImGuiMouseButton_Left)) return;

        Core::Vector2 snappedPos = GetSnappedWorldPos();

        if (lineTargetNode.has_value())
        {
            GUID rightNodeID = currentMapData->AddNode(snappedPos);
            lastCreatedWallID = CreateWall(*lineTargetNode, rightNodeID);
            if (commandHistory) commandHistory->Push(std::make_unique<PlaceLineSegmentCommand>(
                currentMapData->GetWall(*lastCreatedWallID), snappedPos));
            lineTargetNode = rightNodeID;
            return;
        }

        lineTargetNode = currentMapData->AddNode(snappedPos);
        if (commandHistory) commandHistory->Push(std::make_unique<PlaceNodeCommand>(*lineTargetNode, snappedPos));
    }

    void MapView::HandleHotKeys()
    {
        const MapEditorHotKeys& hotKeys = ConfigurationManager::INS.GetMapEditorHotKeys();
        if (!hotKeys.cancelAction.IsKeyPressed(false) || !lineTargetNode.has_value()) return;

        if (!lastCreatedWallID.has_value() && commandHistory)
            commandHistory->Undo(*currentMapData);

        lineTargetNode = lastCreatedWallID = std::nullopt;
    }

    GUID MapView::CreateWall(GUID leftNodeID, GUID rightNodeID)
    {
        const DrawingData& drawingData = selectionManager->GetDrawingData();

        EditorWall wall = EditorWall
        {
            leftNodeID, rightNodeID,
            drawingData.wallTopColor, drawingData.wallInnerColor, drawingData.wallBtmColor,
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
        if (!isDrawingLine || !lineTargetNode.has_value()) return;

        const DrawingLineTheme& lineTheme = ConfigurationManager::INS.GetLineTheme();
        const DrawingNodeTheme& nodeTheme = ConfigurationManager::INS.GetNodeTheme();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const EditorNode& node = currentMapData->GetNode(*lineTargetNode);

        Core::Vector2 screenNodePos = grid->WorldToScreen(node.pos);
        Core::Vector2 offset = Core::Vector2::ONE * nodeTheme.selectedNodePointThickness;
        Core::Vector2 min = screenNodePos - offset, max = screenNodePos + offset;

        drawList->AddRectFilled(min, max, ImGui::GetColorU32(nodeTheme.selectedNodePoint));

        drawList->AddLine(screenNodePos, GetScreenSpaceCursor(), ImGui::GetColorU32(lineTheme.drawingVectorLine),
            lineTheme.drawingLineThickness);
    }

    void MapView::SyncAfterUndo(std::optional<std::reference_wrapper<const IEditorCommand>> cmd)
    {
        if (!isDrawingLine) return;

        if (cmd.has_value())
        {
            auto target = cmd->get().GetRestoredUndoTarget();
            if (target.has_value())
            {
                lineTargetNode = *target;
                lastCreatedWallID = std::nullopt;
                return;
            }
        }

        if (!lineTargetNode.has_value()) return;
        if (!currentMapData->HasNode(*lineTargetNode))
            lineTargetNode = lastCreatedWallID = std::nullopt;
    }

    void MapView::SyncAfterRedo(std::optional<std::reference_wrapper<const IEditorCommand>> cmd)
    {
        if (!isDrawingLine || !cmd.has_value()) return;
        auto target = cmd->get().GetRestoredLineTarget();
        if (target.has_value()) lineTargetNode = *target;
        lastCreatedWallID = cmd->get().GetRestoredWallID();
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

    void MapData::RefreshWallBoundsForNode(GUID nodeID)
    {
        for (auto& [wallID, wall] : walls)
        {
            if (wall.leftPoint == nodeID || wall.rightPoint == nodeID)
            {
                const EditorNode& leftNode = GetNode(wall.leftPoint);
                const EditorNode& rightNode = GetNode(wall.rightPoint);
                wall.UpdateBounds(leftNode.pos, rightNode.pos);
            }
        }
    }
}
