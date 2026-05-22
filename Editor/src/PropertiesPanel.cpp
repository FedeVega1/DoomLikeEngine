#include "pch.h"

#include <imgui.h>

#include "Editor/EditorTypes.h"
#include "Editor/EditorCommands.h"
#include "Editor/SelectionManager.h"
#include "Editor/Panels/PropertiesPanel.h"

namespace Editor::Panels
{
    void PropertiesPanel::Render(SelectionManager& selection, CommandPtr onCommand)
    {
        if (selection.IsOnDrawingMode())
        {
            DrawLineDrawingProperties(selection.GetDrawingData(), onCommand);
            return;
        }

        const std::vector<SelectedItem>& currentSelection = selection.GetSelected();
        if (currentSelection.empty()) { ImGui::TextDisabled("Nothing selected"); return; }

        MapData& data = selection.GetMapData();

        if (currentSelection.size() == 1)
        {
            const SelectedItem& item = currentSelection[0];
            if (item.type == SelectableType::Node) { DrawNodeProperties(data.GetNode(item.id), onCommand); return; }
            if (item.type == SelectableType::Wall) { DrawWallProperties(data.GetWall(item.id), onCommand); return; }
            if (item.type == SelectableType::Sector) { DrawSectorProperties(data.GetSector(item.id), onCommand); return; }
            return;
        }

        ImGui::Text("%d objects selected", static_cast<int>(currentSelection.size()));
        ImGui::Separator();

        SelectableType firstType = currentSelection[0].type;
        bool allSameType = std::all_of(currentSelection.begin(), currentSelection.end(),
            [firstType](const SelectedItem& i) { return i.type == firstType; });

        if (!allSameType) return;

        std::vector<SelectedItem> items = selection.GetSelectedOfType(firstType);

        if (firstType == SelectableType::Wall) DrawMultiWallProperties(data, items, onCommand);
        else if (firstType == SelectableType::Sector) DrawMultiSectorProperties(data, items, onCommand);
        else if (firstType == SelectableType::Node) DrawMultiNodeProperties(data, items, onCommand);
    }

    void PropertiesPanel::DrawSectorProperties(EditorSector& sector, CommandPtr& onCommand)
    {
        ImGui::SeparatorText("Sector");
        ImGui::Text("ID: %u", sector.sectorID);

        float oldFloor = sector.floorHeight;
        ImGui::DragFloat("Floor Height", &sector.floorHeight, 1.f);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && sector.floorHeight != oldFloor)
            onCommand(std::make_unique<SectorFloorHeightCommand>(sector.sectorID, oldFloor, sector.floorHeight));

        float oldCeiling = sector.ceilingHeight;
        ImGui::DragFloat("Ceiling Height", &sector.ceilingHeight, 1.f);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && sector.ceilingHeight != oldCeiling)
            onCommand(std::make_unique<SectorCeilingHeightCommand>(sector.sectorID, oldCeiling, sector.ceilingHeight));

        float floorColor[3] = { sector.floorColor.r / 255.f, sector.floorColor.g / 255.f, sector.floorColor.b / 255.f };
        Core::Color oldFloorColor = sector.floorColor;

        if (ImGui::ColorEdit3("Floor Color", floorColor)) sector.floorColor = Core::Color(static_cast<uint8_t>(floorColor[0] * 255),
            static_cast<uint8_t>(floorColor[1] * 255), static_cast<uint8_t>(floorColor[2] * 255));

        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(sector.floorColor == oldFloorColor))
            onCommand(std::make_unique<SectorFloorColorCommand>(sector.sectorID, oldFloorColor, sector.floorColor));

        float ceilColor[3] = { sector.ceilingColor.r / 255.f, sector.ceilingColor.g / 255.f, sector.ceilingColor.b / 255.f };
        Core::Color oldCeilingColor = sector.ceilingColor;

        if (ImGui::ColorEdit3("Ceiling Color", ceilColor)) sector.ceilingColor = Core::Color(static_cast<uint8_t>(ceilColor[0] * 255),
            static_cast<uint8_t>(ceilColor[1] * 255), static_cast<uint8_t>(ceilColor[2] * 255));

        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(sector.ceilingColor == oldCeilingColor))
            onCommand(std::make_unique<SectorCeilingColorCommand>(sector.sectorID, oldCeilingColor, sector.ceilingColor));
    }

    void PropertiesPanel::DrawWallProperties(EditorWall& wall, CommandPtr& onCommand)
    {
        ImGui::SeparatorText("Wall");
        ImGui::Text("ID: %llu", wall.wallID);

        Core::Color oldTopColor = wall.topColor;
        EditorColor("Top Color", wall.topColor);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && wall.topColor != oldTopColor)
            onCommand(std::make_unique<WallColorCommand>(wall.wallID, 0, oldTopColor, wall.topColor));

        Core::Color oldInnerColor = wall.innerColor;
        EditorColor("Inner Color", wall.innerColor);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && wall.innerColor != oldInnerColor)
            onCommand(std::make_unique<WallColorCommand>(wall.wallID, 1, oldInnerColor, wall.innerColor));

        Core::Color oldBottomColor = wall.bottomColor;
        EditorColor("Bottom Color", wall.bottomColor);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && wall.bottomColor != oldBottomColor)
            onCommand(std::make_unique<WallColorCommand>(wall.wallID, 2, oldBottomColor, wall.bottomColor));

        ImGui::Separator();
        ImGui::Checkbox("Portal", &wall.isPortal);
        ImGui::Checkbox("Connection", &wall.isConnection);

        if (!wall.isPortal) return;
        ImGui::Text("Target Sector: %u", wall.portalTargetSectorID);
        ImGui::Text("Target Wall: %llu", wall.portalWallTargetID);
    }

    void PropertiesPanel::DrawNodeProperties(EditorNode& node, CommandPtr& onCommand)
    {
        ImGui::SeparatorText("Node");
        ImGui::Text("ID: %llu", node.nodeID);

        float pos[2] = { node.pos.x, node.pos.y };
        ImGui::DragFloat2("Position", pos);

        node.pos.x = pos[0];
        node.pos.y = pos[1];
    }

    void PropertiesPanel::DrawMultiSectorProperties(MapData& data, const std::vector<SelectedItem>& items, CommandPtr& onCommand)
    {
        ImGui::SeparatorText("Sector");

        float floorHeight = data.GetSector(items[0].id).floorHeight;
        bool floorMixed = false;
        for (size_t i = 1; i < items.size(); i++)
        {
            if (data.GetSector(items[i].id).floorHeight == floorHeight) continue;
            floorMixed = true;
            break;
        }

        float displayFloor = floorMixed ? 0.f : floorHeight, oldFloor = displayFloor;
        ImGui::DragFloat("Floor Height", &displayFloor, 1.f);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && displayFloor != oldFloor)
        {
            auto cmd = std::make_unique<CompositeCommand>("Change Floor Height");
            for (const auto& item : items)
                cmd->steps.push_back(std::make_unique<SectorFloorHeightCommand>(item.id, data.GetSector(item.id).floorHeight, displayFloor));
            onCommand(std::move(cmd));
        }

        float ceilHeight = data.GetSector(items[0].id).ceilingHeight;
        bool ceilMixed = false;
        for (size_t i = 1; i < items.size(); i++)
        {
            if (data.GetSector(items[i].id).ceilingHeight == ceilHeight) continue;
            ceilMixed = true; 
            break;
        }

        float displayCeil = ceilMixed ? 0.f : ceilHeight;
        float oldCeil = displayCeil;
        ImGui::DragFloat("Ceiling Height", &displayCeil, 1.f);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && displayCeil != oldCeil)
        {
            auto cmd = std::make_unique<CompositeCommand>("Change Ceiling Height");
            for (const auto& item : items)
                cmd->steps.push_back(std::make_unique<SectorCeilingHeightCommand>(item.id, data.GetSector(item.id).ceilingHeight, displayCeil));
            onCommand(std::move(cmd));
        }

        Core::Color floorColor = data.GetSector(items[0].id).floorColor;
        bool floorColorMixed = false;
        for (size_t i = 1; i < items.size(); i++)
        {
            if (data.GetSector(items[i].id).floorColor == floorColor) continue;
            floorColorMixed = true;
            break;
        }

        Core::Color displayFloorColor = floorColorMixed ? Core::Color::Black : floorColor, oldFloorColor = displayFloorColor;
        EditorColor("Floor Color", displayFloorColor);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(displayFloorColor == oldFloorColor))
        {
            auto cmd = std::make_unique<CompositeCommand>("Change Floor Color");
            for (const auto& item : items)
                cmd->steps.push_back(std::make_unique<SectorFloorColorCommand>(item.id, data.GetSector(item.id).floorColor, displayFloorColor));
            onCommand(std::move(cmd));
        }

        Core::Color ceilColor = data.GetSector(items[0].id).ceilingColor;
        bool ceilColorMixed = false;
        for (size_t i = 1; i < items.size(); i++)
        {
            if (data.GetSector(items[i].id).ceilingColor == ceilColor) continue;
            ceilColorMixed = true; 
            break;
        }

        Core::Color displayCeilColor = ceilColorMixed ? Core::Color::Black : ceilColor, oldCeilColor = displayCeilColor;
        EditorColor("Ceiling Color", displayCeilColor);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(displayCeilColor == oldCeilColor))
        {
            auto cmd = std::make_unique<CompositeCommand>("Change Ceiling Color");
            for (const auto& item : items)
                cmd->steps.push_back(std::make_unique<SectorCeilingColorCommand>(item.id, data.GetSector(item.id).ceilingColor, displayCeilColor));
            onCommand(std::move(cmd));
        }
    }

    void PropertiesPanel::DrawMultiWallProperties(MapData& data, const std::vector<SelectedItem>& items, CommandPtr& onCommand)
    {
        ImGui::SeparatorText("Wall");

        Core::Color topColor = data.GetWall(items[0].id).topColor;
        bool topMixed = false;
        for (size_t i = 1; i < items.size(); i++)
        {
            if (data.GetWall(items[i].id).topColor == topColor) continue;
            topMixed = true; 
            break;
        }

        Core::Color displayTop = topMixed ? Core::Color::Black : topColor, oldTop = displayTop;
        EditorColor("Top Color", displayTop);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(displayTop == oldTop))
        {
            auto cmd = std::make_unique<CompositeCommand>("Change Wall Top Color");
            for (const auto& item : items)
                cmd->steps.push_back(std::make_unique<WallColorCommand>(item.id, 0, data.GetWall(item.id).topColor, displayTop));
            onCommand(std::move(cmd));
        }

        Core::Color innerColor = data.GetWall(items[0].id).innerColor;
        bool innerMixed = false;
        for (size_t i = 1; i < items.size(); i++)
        {
            if (data.GetWall(items[i].id).innerColor == innerColor) continue;
            innerMixed = true; 
            break;
        }

        Core::Color displayInner = innerMixed ? Core::Color::Black : innerColor, oldInner = displayInner;
        EditorColor("Inner Color", displayInner);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(displayInner == oldInner))
        {
            auto cmd = std::make_unique<CompositeCommand>("Change Wall Inner Color");
            for (const auto& item : items)
                cmd->steps.push_back(std::make_unique<WallColorCommand>(item.id, 1, data.GetWall(item.id).innerColor, displayInner));
            onCommand(std::move(cmd));
        }

        Core::Color bottomColor = data.GetWall(items[0].id).bottomColor;
        bool bottomMixed = false;
        for (size_t i = 1; i < items.size(); i++)
        {
            if (data.GetWall(items[i].id).bottomColor == bottomColor) continue;
            bottomMixed = true; 
            break;
        }

        Core::Color displayBottom = bottomMixed ? Core::Color::Black : bottomColor, oldBottom = displayBottom;
        EditorColor("Bottom Color", displayBottom);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(displayBottom == oldBottom))
        {
            auto cmd = std::make_unique<CompositeCommand>("Change Wall Bottom Color");
            for (const auto& item : items)
                cmd->steps.push_back(std::make_unique<WallColorCommand>(item.id, 2, data.GetWall(item.id).bottomColor, displayBottom));
            onCommand(std::move(cmd));
        }
    }

    void PropertiesPanel::DrawMultiNodeProperties(MapData& data, const std::vector<SelectedItem>& items, CommandPtr& onCommand)
    {
        ImGui::SeparatorText("Node");

        Core::Vector2 firstPos = data.GetNode(items[0].id).pos;
        bool xMixed = false, yMixed = false;
        for (size_t i = 1; i < items.size(); i++)
        {
            const Core::Vector2& pos = data.GetNode(items[i].id).pos;
            if (pos.x != firstPos.x) xMixed = true;
            if (pos.y != firstPos.y) yMixed = true;
        }

        float displayPos[2] = { xMixed ? 0.f : firstPos.x, yMixed ? 0.f : firstPos.y };
        float oldPos[2] = { displayPos[0], displayPos[1] };

        ImGui::DragFloat2("Position", displayPos);

        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && (displayPos[0] != oldPos[0] || displayPos[1] != oldPos[1]))
        {
            auto cmd = std::make_unique<GeometryDragCommand>("Move Nodes");
            for (const auto& item : items)
            {
                const Core::Vector2& nodePos = data.GetNode(item.id).pos;
                cmd->deltas.push_back({ item.id, nodePos, Core::Vector2(displayPos[0], displayPos[1]) });
            }

            onCommand(std::move(cmd));
        }
    }

    void PropertiesPanel::EditorColor(const std::string& label, Core::Color& color)
    {
        float colors[3] = { color.r / 255.f, color.g / 255.f, color.b / 255.f };
        if (!ImGui::ColorEdit3(label.c_str(), colors)) return;
        color = Core::Color(static_cast<uint8_t>(colors[0] * 255), static_cast<uint8_t>(colors[1] * 255),
            static_cast<uint8_t>(colors[2] * 255));
    }

    void PropertiesPanel::EditorColorCompact(const std::string& label, Core::Color& color)
    {
        float colors[3] = { color.r / 255.f, color.g / 255.f, color.b / 255.f };
        if (!ImGui::ColorEdit3(label.c_str(), colors, ImGuiColorEditFlags_NoInputs)) return;
        color = Core::Color(static_cast<uint8_t>(colors[0] * 255), static_cast<uint8_t>(colors[1] * 255),
            static_cast<uint8_t>(colors[2] * 255));
    }

    void PropertiesPanel::DrawLineDrawingProperties(DrawingData& data, CommandPtr& onCommand)
    {
        ImGui::SeparatorText("New Wall Data");
        EditorColorCompact("Top Color", data.wallTopColor);
        EditorColorCompact("Inner Color", data.wallInnerColor);
        EditorColorCompact("Bottom Color", data.wallBtmColor);

        ImGui::SeparatorText("New Sector Data");

        ImGui::Text("Ceiling");
        ImGui::SameLine();

        EditorColorCompact("##Ceiling Color", data.sectorCeilingColor);
        ImGui::SameLine();

        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("##Ceiling Height", &data.sectorCeilingHeight);

        ImGui::Text("Floor");
        ImGui::SameLine();

        EditorColorCompact("##Floor Color", data.sectorFloorColor);
        ImGui::SameLine();

        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("##Floor Height", &data.sectorFloorHeight);
    }
}
