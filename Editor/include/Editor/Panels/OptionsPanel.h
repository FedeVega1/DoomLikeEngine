#pragma once

namespace Editor::Panels
{
	struct HotKeyOption
	{
		std::string optionName;
		ImGuiKey* value, * modifierKey;
		int selectionIndx, modifierSelectionIndx;
	};

	class HotKeyOptionsHelper
	{
	public:
		HotKeyOptionsHelper() = default;
		HotKeyOptionsHelper(std::vector<HotKeyOption> options);
		~HotKeyOptionsHelper() = default;

		void Render();

	private:
		void InitializeMaps();

		std::vector<HotKeyOption> options;

		static std::vector<std::string> keyMapStrings;
		static std::map<std::string, ImGuiKey> keyMap;

		static std::vector<std::string> modifierStrings;
		static std::map<std::string, ImGuiKey> modifierMap;
	};

	struct ColorOption
	{
		std::string optionName;
		ImVec4* colorValue;
	};

	class ColorOptionsHelper
	{
	public:
		ColorOptionsHelper() = default;
		ColorOptionsHelper(std::vector<ColorOption> options) { this->options = options; }
		~ColorOptionsHelper() = default;
		
		void Render();

	private:

		std::vector<ColorOption> options;
	};

	class OptionsSection
	{
	public:
		OptionsSection(const std::string& name);
		~OptionsSection() = default;

		void Render();

	protected:
		virtual void InternalRender() = 0;

		std::string sectionName;
	};

	class GeneralSection : public OptionsSection
	{
	public:
		GeneralSection() : OptionsSection("General") {}

	protected:
		void InternalRender() override;
	};

	class GridSection : public OptionsSection
	{
	public:
		GridSection();

	protected:
		void InternalRender() override;

	private:
		int currentSelectedGridOption;

		std::array<const char*, 9> gridSizeOptions;
		HotKeyOptionsHelper hotKeyHelper;
		ColorOptionsHelper colorHelper;
	};

	enum OptionSectionType { General, Grid };

	class OptionsPanel
	{
		friend OptionsSection;

	public:
		OptionsPanel();
		~OptionsPanel() = default;

		void Toggle(bool toggle) { isActive = toggle; }
		void Render();

	private:
		std::map<OptionSectionType, std::unique_ptr<OptionsSection>> sections;
		bool isActive;
	};
}
