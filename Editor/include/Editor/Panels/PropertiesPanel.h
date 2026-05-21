#pragma once

namespace Editor::Panels
{
    class PropertiesPanel
    {
    public:
        void Render(SelectionManager& selection,
            std::function<void(std::unique_ptr<IEditorCommand>)> onCommand = nullptr);

    private:
        using CommandPtr = std::function<void(std::unique_ptr<IEditorCommand>)>;

        void DrawSectorProperties(EditorSector& sector, CommandPtr& onCommand);
        void DrawWallProperties(EditorWall& wall, CommandPtr& onCommand);
        void DrawNodeProperties(EditorNode& node, CommandPtr& onCommand);

        void DrawMultiSectorProperties(MapData& data, const std::vector<SelectedItem>& items, CommandPtr& onCommand);
        void DrawMultiWallProperties(MapData& data, const std::vector<SelectedItem>& items, CommandPtr& onCommand);
        void DrawMultiNodeProperties(MapData& data, const std::vector<SelectedItem>& items, CommandPtr& onCommand);
        
        void DrawLineDrawingProperties(DrawingData& data, CommandPtr& onCommand);

        void EditorColor(const std::string& label, Core::Color& color);
        void EditorColorCompact(const std::string& label, Core::Color& color);
    };
}
