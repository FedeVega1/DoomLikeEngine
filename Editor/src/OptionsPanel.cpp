#include "pch.h"

#include <imgui.h>

#include "Editor/OptionsPanel.h"

namespace Editor::Panels
{
	OptionsSection::OptionsSection(std::string name) : isActive(false)
	{
		sectionName = name;
	}

	void OptionsSection::Render()
	{
		if (!isActive) return;
		ImGui::Begin(sectionName.c_str());

		ImGui::End();
	}

	OptionsPanel::OptionsPanel() : isActive(false)
	{
		sections = std::map<OptionSectionType, std::unique_ptr<OptionsSection>>();

		sections.emplace(General, std::make_unique<OptionsSection>("General"));
		sections.emplace(Grid, std::make_unique<OptionsSection>("Grid"));
		sections[General]->Toggle(true);
	}

	void OptionsPanel::Render()
	{
		if (!isActive) return;
		ImGui::Begin("Options");

		for (const std::pair<const OptionSectionType, std::unique_ptr<OptionsSection>>& pair : sections)
			pair.second->Render();

		ImGui::End();
	}
}
