#include "pch.h"

#include <imgui.h>

#include "Editor/Config/EditorConfiguration.h"
#include "Editor/Panels/OptionsPanel.h"

ConfigurationManager ConfigurationManager::INS = ConfigurationManager();

namespace Editor::Panels
{
	OptionsSection::OptionsSection(const std::string& name)
	{
		sectionName = name;
	}

	void OptionsSection::Render()
	{
		if (!ImGui::BeginTabItem(sectionName.c_str())) return;
		InternalRender();
		ImGui::EndTabItem();
	}

	OptionsPanel::OptionsPanel() : isActive(false)
	{
		sections = std::map<OptionSectionType, std::unique_ptr<OptionsSection>>();

		sections.emplace(General, std::make_unique<GeneralSection>());
		sections.emplace(Grid, std::make_unique<GridSection>());
	}

	void OptionsPanel::Render()
	{
		if (!isActive) return;
		ImGui::Begin("Options", &isActive, ImGuiWindowFlags_NoDocking);

		if (ImGui::BeginTabBar("OptionsTabs"))
		{
			for (const std::pair<const OptionSectionType, std::unique_ptr<OptionsSection>>& pair : sections)
				pair.second->Render();

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	void GeneralSection::InternalRender()
	{

	}

	GridSection::GridSection() : OptionsSection("Grid"), currentSelectedGridOption(4)
	{
		gridSizeOptions = { "1", "2", "4", "8", "16", "32", "64", "128", "256" };
	}

	void GridSection::InternalRender()
	{
		ImGui::Combo("Default Grid Size", &currentSelectedGridOption, gridSizeOptions.data(), gridSizeOptions.size());
		ConfigurationManager::INS.GetGridConfig().defaultGridSize = std::stoi(gridSizeOptions[currentSelectedGridOption]);

		ImGui::SliderFloat("Default Zoom Level", &ConfigurationManager::INS.GetGridConfig().defaultZoom, .1f, 1);
		ImGui::Checkbox("Dotted grid", &ConfigurationManager::INS.GetGridConfig().dottedLines);
	}
}
