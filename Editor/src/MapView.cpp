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
    MapView::MapView() : firstRender(false), isDrawingLine(false), isHoveringWindow(false), cursorTime(0.f), commandHistory(nullptr),
        currentMapData(std::make_unique<MapData>(0UL, 0UL, 0UL)), selectionManager(nullptr)
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

        lineTargetNode = lastCreatedWallID = firstNodeID = std::nullopt;
        isDrawingLine = firstRender = false;
    }

    void MapView::LoadSectors(std::vector<EditorSector> loadedSectors)
    {
        /*sectors = std::move(loadedSectors);*/
        currentMapData = std::make_unique<MapData>(0UL, 0UL, 0UL);
        lineTargetNode = lastCreatedWallID = firstNodeID = std::nullopt;
        isDrawingLine = firstRender = false;
    }

    void MapView::HandleInput()
    {
        grid->HandleInputsNoFocus();
        if (!isHoveringWindow) return;
        grid->HandleInputs();
        HandleHotKeys();

        if (!isDrawingLine || !ImGui::IsMouseClicked(ImGuiMouseButton_Left)) return;
        PlaceNewNode(GetSnappedWorldPos());
    }

    void MapView::HandleHotKeys()
    {
        const MapEditorHotKeys& hotKeys = ConfigurationManager::INS.GetMapEditorHotKeys();
        if (!hotKeys.cancelAction.IsKeyPressed(false) || !lineTargetNode.has_value()) return;

        if (!lastCreatedWallID.has_value() && commandHistory)
            commandHistory->Undo(*currentMapData);

        lineTargetNode = lastCreatedWallID = std::nullopt;
    }

    bool MapView::IsLastDrawPolygonConvex() const
    {
        if (!firstNodeID.has_value()) return false;

        auto firstWall = currentMapData->FindWallByNodeID(firstNodeID.value());
        if (!firstWall.has_value()) return false;

        const EditorWall* currentWall = &firstWall.value().get();
        int lastCrossSign = 0;

        do
        {
            auto nextWall = currentMapData->FindWallByNodeID(currentWall->rightNodeID);
            if (!nextWall.has_value()) return false;

            const EditorWall& targetWall = nextWall.value().get();

            const EditorNode& nodeA = currentMapData->GetNode(currentWall->leftNodeID);
            const EditorNode& nodeB = currentMapData->GetNode(currentWall->rightNodeID);
            const EditorNode& nodeC = currentMapData->GetNode(targetWall.rightNodeID);

            currentWall = &targetWall;
            float cross = Core::Vector2::Cross(nodeB.pos - nodeA.pos, nodeC.pos - nodeB.pos);
            if (std::abs(cross) < Core::K_EPSILON) continue;

            int sign = cross > 0 ? 1 : -1;

            if (lastCrossSign != 0 && sign != lastCrossSign) return false;
            lastCrossSign = sign;


        } while (currentWall->leftNodeID != firstNodeID.value());

        return true;
    }

    void MapView::PlaceNewNode(const Core::Vector2& pos)
    {
        std::optional<GUID> overlappingNode = IsPosOveralppingNode(pos);

        if (lineTargetNode.has_value())
        {
            GUID rightNodeID = overlappingNode.has_value() ? overlappingNode.value() : currentMapData->AddNode(pos);
            lastCreatedWallID = CreateWall(*lineTargetNode, rightNodeID);

            if (commandHistory) commandHistory->Push(std::make_unique<PlaceLineSegmentCommand>(
                currentMapData->GetWall(*lastCreatedWallID), pos));

            std::optional<GUID> overlappingWall = IsPosOveralppingWall(pos);
            if (overlappingWall)
            {
                // TODO
            }

            if (!overlappingNode.has_value())
            {
                lineTargetNode = rightNodeID;
                return;
            }

            CreateSector();
            lineTargetNode = std::nullopt;
            return;
        }

        if (overlappingNode.has_value()) lineTargetNode = overlappingNode.value();
        else
        {
            std::optional<GUID> overlappingWall = IsPosOveralppingWall(pos);
            if (overlappingWall)
            {
                // TODO
                return;
            }

            lineTargetNode = currentMapData->AddNode(pos);
            if (commandHistory) commandHistory->Push(std::make_unique<PlaceNodeCommand>(*lineTargetNode, pos));
        }

        if (!firstNodeID.has_value()) firstNodeID = lineTargetNode;
    }

    std::optional<GUID> MapView::IsPosOveralppingNode(const Core::Vector2& pos) const
    {
        std::optional<GUID> overlappingNode = std::nullopt;

        currentMapData->ForEachNode([&](const EditorNode& node) 
        {  
            if (Core::Vector2::Abs(node.pos - pos).Magnitude() >= Core::K_EPSILON) return;
            overlappingNode = node.nodeID;
        });

        return std::nullopt;
    }

    std::optional<GUID> MapView::IsPosOveralppingWall(const Core::Vector2& pos) const
    {
        std::optional<GUID> overlappingWall = std::nullopt;

        currentMapData->ForEachWall([&](const EditorWall& wall)
        {
            const EditorNode& leftNode = currentMapData->GetNode(wall.leftNodeID);
            const EditorNode& rightNode = currentMapData->GetNode(wall.rightNodeID);

            Core::Vector2 ab = rightNode.pos - leftNode.pos;
            Core::Vector2 ap = pos - leftNode.pos;

            float cross = Core::Vector2::Cross(ab, ap);
            if (std::abs(cross) > Core::K_EPSILON) return;

            float dot = Core::Vector2::Dot(ab, ap);
            if (dot >= .0f && dot <= 1.f) overlappingWall = wall.wallID;
        });

        return overlappingWall;
    }

    GUID MapView::CreateSector()
    {
        const DrawingData& drawingData = selectionManager->GetDrawingData();

        std::vector<GUID> sectorWalls = currentMapData->FindSectorWallsByFirstNode(firstNodeID.value());

        EditorSector sector = EditorSector
        {
            Core::NULL_ID_32,
            drawingData.sectorFloorHeight, drawingData.sectorCeilingHeight,
            drawingData.sectorFloorColor, drawingData.sectorCeilingColor,
            sectorWalls, Core::Vector2::ZERO, Core::Vector2::ZERO
        };

        return currentMapData->AddSector(sector);
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
}
