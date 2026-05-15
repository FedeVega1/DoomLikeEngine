#pragma once

namespace Editor::Panels
{
    class PropertiesPanel
    {
    public:
        void Render(EditorSector* selectedSector, EditorWall* selectedWall);

    private:
        void DrawSectorProperties(EditorSector& sector);
        void DrawWallProperties(EditorWall& wall);
        void EditorColor(const std::string& label, Core::Color& color);
    };
}
