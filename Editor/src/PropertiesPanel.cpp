#include "pch.h"

#include <imgui.h>

#include "Editor/EditorTypes.h"
#include "Editor/EditorCommands.h"
#include "Editor/Panels/PropertiesPanel.h"

namespace Editor::Panels
{
    void PropertiesPanel::Render(std::optional<std::reference_wrapper<EditorSector>> selectedSector,
                                 std::optional<std::reference_wrapper<EditorWall>> selectedWall,
                                 std::function<void(std::unique_ptr<IEditorCommand>)> onCommand)
    {
        if (selectedWall.has_value())
        {
            DrawWallProperties(selectedWall->get(), onCommand);
            return;
        }

        if (selectedSector.has_value())
        {
            DrawSectorProperties(selectedSector->get(), onCommand);
            return;
        }

        ImGui::TextDisabled("Nothing selected");
    }

    void PropertiesPanel::DrawSectorProperties(EditorSector& sector,
                                               std::function<void(std::unique_ptr<IEditorCommand>)>& onCommand)
    {
        ImGui::SeparatorText("Sector");
        ImGui::Text("ID: %u", sector.sectorID);

        float oldFloor = sector.floorHeight;
        ImGui::DragFloat("Floor Height", &sector.floorHeight, 1.f);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && sector.floorHeight != oldFloor)
        {
            onCommand(std::make_unique<SectorFloorHeightCommand>(sector.sectorID, oldFloor, sector.floorHeight));
        }

        float oldCeiling = sector.ceillingHeight;
        ImGui::DragFloat("Ceiling Height", &sector.ceillingHeight, 1.f);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && sector.ceillingHeight != oldCeiling)
        {
            onCommand(std::make_unique<SectorCeilingHeightCommand>(sector.sectorID, oldCeiling, sector.ceillingHeight));
        }

        float floorColor[3] = { sector.floorColor.r / 255.f, sector.floorColor.g / 255.f, sector.floorColor.b / 255.f };
        Core::Color oldFloorColor = sector.floorColor;
        if (ImGui::ColorEdit3("Floor Color", floorColor))
            sector.floorColor = Core::Color(static_cast<uint8_t>(floorColor[0] * 255), static_cast<uint8_t>(floorColor[1] * 255), static_cast<uint8_t>(floorColor[2] * 255));
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(sector.floorColor == oldFloorColor))
        {
            onCommand(std::make_unique<SectorFloorColorCommand>(sector.sectorID, oldFloorColor, sector.floorColor));
        }

        float ceilColor[3] = { sector.ceillingColor.r / 255.f, sector.ceillingColor.g / 255.f, sector.ceillingColor.b / 255.f };
        Core::Color oldCeilingColor = sector.ceillingColor;
        if (ImGui::ColorEdit3("Ceiling Color", ceilColor))
            sector.ceillingColor = Core::Color(static_cast<uint8_t>(ceilColor[0] * 255), static_cast<uint8_t>(ceilColor[1] * 255), static_cast<uint8_t>(ceilColor[2] * 255));
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(sector.ceillingColor == oldCeilingColor))
        {
            onCommand(std::make_unique<SectorCeilingColorCommand>(sector.sectorID, oldCeilingColor, sector.ceillingColor));
        }
    }

    void PropertiesPanel::DrawWallProperties(EditorWall& wall,
                                             std::function<void(std::unique_ptr<IEditorCommand>)>& onCommand)
    {
        ImGui::SeparatorText("Wall");
        ImGui::Text("ID: %llu", wall.wallID);

        Core::Color oldTopColor = wall.topColor;
        EditorColor("Top Color", wall.topColor);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(wall.topColor == oldTopColor))
        {
            onCommand(std::make_unique<WallColorCommand>(wall.wallID, 0, oldTopColor, wall.topColor));
        }

        Core::Color oldInnerColor = wall.innerColor;
        EditorColor("Inner Color", wall.innerColor);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(wall.innerColor == oldInnerColor))
        {
            onCommand(std::make_unique<WallColorCommand>(wall.wallID, 1, oldInnerColor, wall.innerColor));
        }

        Core::Color oldBottomColor = wall.bottomColor;
        EditorColor("Bottom Color", wall.bottomColor);
        if (ImGui::IsItemDeactivatedAfterEdit() && onCommand && !(wall.bottomColor == oldBottomColor))
        {
            onCommand(std::make_unique<WallColorCommand>(wall.wallID, 2, oldBottomColor, wall.bottomColor));
        }

        ImGui::Separator();
        ImGui::Checkbox("Portal", &wall.isPortal);
        ImGui::Checkbox("Connection", &wall.isConnection);

        if (!wall.isPortal) return;
        ImGui::Text("Target Sector: %u", wall.portalTargetSectorID);
        ImGui::Text("Target Wall: %llu", wall.portalWallTargetID);
    }

    void PropertiesPanel::EditorColor(const std::string& label, Core::Color& color)
    {
        float colors[3] = { color.r / 255.f, color.g / 255.f, color.b / 255.f };
        if (!ImGui::ColorEdit3(label.c_str(), colors)) return;
        color = Core::Color(static_cast<uint8_t>(colors[0] * 255), static_cast<uint8_t>(colors[1] * 255), static_cast<uint8_t>(colors[2] * 255));
    }
}
