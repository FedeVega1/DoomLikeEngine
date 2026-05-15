#include "pch.h"

#include <imgui.h>

#include "Editor/EditorTypes.h"
#include "Editor/Panels/PropertiesPanel.h"

namespace Editor::Panels
{
    void PropertiesPanel::Render(EditorSector* selectedSector, EditorWall* selectedWall)
    {
        if (selectedWall)
        {
            DrawWallProperties(*selectedWall);
            return;
        }

        if (selectedSector)
        {
            DrawSectorProperties(*selectedSector);
            return;
        }

        ImGui::TextDisabled("Nothing selected");
    }

    void PropertiesPanel::DrawSectorProperties(EditorSector& sector)
    {
        ImGui::SeparatorText("Sector");
        ImGui::Text("ID: %u", sector.sectorID);
        ImGui::DragFloat("Floor Height", &sector.floorHeight, 1.f);
        ImGui::DragFloat("Ceiling Height", &sector.ceillingHeight, 1.f);

        float floorColor[3] = { sector.floorColor.r / 255.f, sector.floorColor.g / 255.f, sector.floorColor.b / 255.f };
        float ceilColor[3] = { sector.ceillingColor.r / 255.f, sector.ceillingColor.g / 255.f, sector.ceillingColor.b / 255.f };

        if (ImGui::ColorEdit3("Floor Color", floorColor))
            sector.floorColor = Core::Color(static_cast<uint8_t>(floorColor[0] * 255), static_cast<uint8_t>(floorColor[1] * 255), static_cast<uint8_t>(floorColor[2] * 255));

        if (ImGui::ColorEdit3("Ceiling Color", ceilColor))
            sector.ceillingColor = Core::Color(static_cast<uint8_t>(ceilColor[0] * 255), static_cast<uint8_t>(ceilColor[1] * 255), static_cast<uint8_t>(ceilColor[2] * 255));
    }

    void PropertiesPanel::DrawWallProperties(EditorWall& wall)
    {
        ImGui::SeparatorText("Wall");
        ImGui::Text("ID: %llu", wall.wallID);

        EditorColor("Top Color", wall.topColor);
        EditorColor("Inner Color", wall.innerColor);
        EditorColor("Bottom Color",wall.bottomColor);

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
