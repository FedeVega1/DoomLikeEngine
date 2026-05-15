#pragma once

namespace Editor::Panels
{
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
