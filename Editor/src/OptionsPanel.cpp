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
		ImGui::SetNextWindowSize({ 546, 378 }, ImGuiCond_FirstUseEver);
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

		hotKeyHelper = HotKeyOptionsHelper({
			HotKeyOption { "Increase Grid Size", &ConfigurationManager::INS.GetGridHotKeys().increaseGridSize.key, &ConfigurationManager::INS.GetGridHotKeys().increaseGridSize.modifier },
			HotKeyOption { "Decrease Grid Size", &ConfigurationManager::INS.GetGridHotKeys().decreaseGridSize.key, &ConfigurationManager::INS.GetGridHotKeys().decreaseGridSize.modifier },
			HotKeyOption { "Pan Left", &ConfigurationManager::INS.GetGridHotKeys().panHorizontal.negativeValue.key, &ConfigurationManager::INS.GetGridHotKeys().panHorizontal.negativeValue.modifier },
			HotKeyOption { "Pan Right", &ConfigurationManager::INS.GetGridHotKeys().panHorizontal.positiveValue.key, &ConfigurationManager::INS.GetGridHotKeys().panHorizontal.positiveValue.modifier },
			HotKeyOption { "Pan Down", &ConfigurationManager::INS.GetGridHotKeys().panVertical.negativeValue.key, &ConfigurationManager::INS.GetGridHotKeys().panVertical.negativeValue.modifier },
			HotKeyOption { "Pan Up", &ConfigurationManager::INS.GetGridHotKeys().panVertical.positiveValue.key, &ConfigurationManager::INS.GetGridHotKeys().panVertical.positiveValue.modifier },
			HotKeyOption { "Zoom In", &ConfigurationManager::INS.GetGridHotKeys().zoomAxis.positiveValue.key, &ConfigurationManager::INS.GetGridHotKeys().zoomAxis.positiveValue.modifier },
			HotKeyOption { "Zoom Out", &ConfigurationManager::INS.GetGridHotKeys().zoomAxis.negativeValue.key, &ConfigurationManager::INS.GetGridHotKeys().zoomAxis.negativeValue.modifier }
		});

		colorHelper = ColorOptionsHelper({
			ColorOption { "Bakground", &ConfigurationManager::INS.GetGridTheme().gridBackgrund },
			ColorOption { "Even Horizontal Line", &ConfigurationManager::INS.GetGridTheme().gridEvenHorizontalLine },
			ColorOption { "Even Vertical Line", &ConfigurationManager::INS.GetGridTheme().gridEvenVerticalLine },
			ColorOption { "Odd Horizontal Line", &ConfigurationManager::INS.GetGridTheme().gridOddHorizontalLine },
			ColorOption { "Odd Vertical Line", &ConfigurationManager::INS.GetGridTheme().gridOddVerticalLine },
			ColorOption { "Origin Horizontal Line", &ConfigurationManager::INS.GetGridTheme().gridOriginHorizontal },
			ColorOption { "Origin Vertical Line", &ConfigurationManager::INS.GetGridTheme().gridOriginVertical },
		});
	}

	void GridSection::InternalRender()
	{
		ImGui::SetNextItemWidth(100);
		ImGui::Combo("Default Grid Size", &currentSelectedGridOption, gridSizeOptions.data(), gridSizeOptions.size());
		ConfigurationManager::INS.GetGridConfig().defaultGridSize = std::stoi(gridSizeOptions[currentSelectedGridOption]);

		ImGui::SetNextItemWidth(100);
		ImGui::SliderFloat("Default Zoom Level", &ConfigurationManager::INS.GetGridConfig().defaultZoom, .1f, 1);

		ImGui::Checkbox("Dotted grid", &ConfigurationManager::INS.GetGridConfig().dottedLines);

		hotKeyHelper.Render();
		colorHelper.Render();

		ImGui::SetNextItemWidth(100);
		ImGui::InputFloat("Even Line Thickness", &ConfigurationManager::INS.GetGridTheme().gridEvenLineThickness);

		ImGui::SetNextItemWidth(100);
		ImGui::InputFloat("Odd Line Thickness", &ConfigurationManager::INS.GetGridTheme().gridOddLineThickness);

		ImGui::SetNextItemWidth(100);
		ImGui::InputFloat("Origin Line Thickness", &ConfigurationManager::INS.GetGridTheme().gridOriginThickness);
	}

	std::vector<std::string> HotKeyOptionsHelper::keyMapStrings;
	std::map<std::string, ImGuiKey> HotKeyOptionsHelper::keyMap;

	std::vector<std::string> HotKeyOptionsHelper::modifierStrings;
	std::map<std::string, ImGuiKey> HotKeyOptionsHelper::modifierMap;

	HotKeyOptionsHelper::HotKeyOptionsHelper(std::vector<HotKeyOption> options)
	{
		this->options = options;
		InitializeMaps();

		for (HotKeyOption& option : this->options)
		{
			auto findIt = std::find_if(keyMapStrings.begin(), keyMapStrings.end(), [&](std::string name) { return keyMap[name] == *option.value; });
			if (findIt != keyMapStrings.end()) option.selectionIndx = std::distance(keyMapStrings.begin(), findIt);

			auto findItMod = std::find_if(modifierStrings.begin(), modifierStrings.end(), [&](std::string name) { return modifierMap[name] == *option.modifierKey; });
			if (findItMod != modifierStrings.end()) option.modifierSelectionIndx = std::distance(modifierStrings.begin(), findItMod);
		}
	}

	void HotKeyOptionsHelper::InitializeMaps()
	{
		if (keyMap.size() == 0)
		{
			for (ImGuiKey key = ImGuiKey_None; key < ImGuiKey_Oem102; key = static_cast<ImGuiKey>(static_cast<int>(key) + 1))
			{
				std::string keyName = ImGui::GetKeyName(key);
				keyMap.emplace(keyName, key);
				keyMapStrings.push_back(keyName.c_str());

				if (key == ImGuiKey_None) key = ImGuiKey_NamedKey_BEGIN;
			}
		}

		if (modifierMap.size() == 0)
		{
			modifierStrings = { "None", "Ctrl", "Shift", "Alt", "Super" };

			modifierMap.emplace(modifierStrings[0], ImGuiMod_None);
			modifierMap.emplace(modifierStrings[1], ImGuiMod_Ctrl);
			modifierMap.emplace(modifierStrings[2], ImGuiMod_Shift);
			modifierMap.emplace(modifierStrings[3], ImGuiMod_Alt);
			modifierMap.emplace(modifierStrings[4], ImGuiMod_Super);
		}
	}

	void HotKeyOptionsHelper::Render()
	{
		ImGui::SeparatorText("Hotkeys");

		std::vector<const char*> keyStrings;
		for (const std::string& name : keyMapStrings) keyStrings.push_back(name.c_str());

		std::vector<const char*> modStrings;
		for (const std::string& name : modifierStrings) modStrings.push_back(name.c_str());

		for (HotKeyOption& option : options)
		{
			ImGui::Text(option.optionName.c_str());
			ImGui::SameLine();

			ImGui::SetNextItemWidth(150);
			ImGui::Combo(("##key_" + option.optionName).c_str(), &option.selectionIndx, keyStrings.data(), keyStrings.size());
			*option.value = keyMap[keyMapStrings[option.selectionIndx]];

			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			ImGui::Combo(("##mod_" + option.optionName).c_str(), &option.modifierSelectionIndx, modStrings.data(), modStrings.size());
			*option.modifierKey = modifierMap[modifierStrings[option.modifierSelectionIndx]];
		}
	}

	void ColorOptionsHelper::Render()
	{
		ImGui::SeparatorText("Color Pickers");

		for (ColorOption& option : options)
			ImGui::ColorEdit4(option.optionName.c_str(), reinterpret_cast<float*>(option.colorValue));
	}
}
