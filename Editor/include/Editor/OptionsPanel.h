#pragma once

namespace Editor::Panels
{
	class OptionsSection
	{
	public:
		OptionsSection(std::string name);
		~OptionsSection() = default;

		void Toggle(bool toggle) { isActive = toggle; }
		void Render();

	private:
		std::string sectionName;
		bool isActive;
	};

	enum OptionSectionType { General, Grid };

	class OptionsPanel
	{
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
