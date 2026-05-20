#pragma once

namespace Editor
{
    class IEditorCommand;
}

namespace Editor::Panels
{
    class PropertiesPanel
    {
    public:
        void Render(std::optional<std::reference_wrapper<EditorSector>> selectedSector,
                    std::optional<std::reference_wrapper<EditorWall>> selectedWall,
                    std::function<void(std::unique_ptr<IEditorCommand>)> onCommand = nullptr);

    private:
        void DrawSectorProperties(EditorSector& sector,
                                  std::function<void(std::unique_ptr<IEditorCommand>)>& onCommand);
        void DrawWallProperties(EditorWall& wall,
                                std::function<void(std::unique_ptr<IEditorCommand>)>& onCommand);
        void EditorColor(const std::string& label, Core::Color& color);
    };
}
