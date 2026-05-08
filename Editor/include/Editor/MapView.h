#pragma once

namespace Editor
{
    class MapView
    {
    public:
        explicit MapView(SDL_Renderer* renderer);

        void Update();
        void Render();

        void NewMap();
        void LoadSectors(std::vector<EditorSector> sectors);
        const std::vector<EditorSector>& GetSectors() const { return sectors; }

        EditorSector* GetSelectedSector();
        EditorWall* GetSelectedWall();

    private:
        void HandleInput();
        void DrawGrid();
        void DrawSectors();
        void DrawSector(const EditorSector& sector);
        void DrawWall(const EditorWall& wall);

        SDL_Renderer* renderer;

        std::vector<EditorSector> sectors;

        Core::Vector2 origin;
        float zoom = 1.f;
        int gridUnitSize = 20, selectedSectorIndex = -1, selectedWallIndex   = -1;

        bool isDrawingLine;
        Core::Vector2 lineStart, linePreview;
    };
}
