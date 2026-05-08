#pragma once

namespace Editor
{
    enum class EditorMode { None, Line, Node, Wall, Sector };

    class EditorApp
    {
    public:
        EditorApp(const SDL_WindowID& wndID, SDL_Renderer* renderer);
        ~EditorApp();

        void Update();
        void Render();

        void NewMap();
        bool OpenMap(const std::string& filePath);
        bool SaveMap(const std::string& filePath);
        bool CompileAndSave(const std::string& bspFilePath);

        void SetMode(EditorMode mode);
        EditorMode GetMode() const { return currentMode; }

        bool IsRunning() const { return running; }
        void RequestQuit() { running = false; }

    private:
        void DrawMainMenuBar();
        void DrawToolbar();
        void DrawStatusBar();

        SDL_Window* window;
        SDL_Renderer* renderer;

        std::unique_ptr<MapView> mapView;
        std::unique_ptr<PropertiesPanel> propertiesPanel;

        EditorMode currentMode = EditorMode::None;
        std::string currentFilePath;
        bool unsavedChanges = false, running = true;
    };
}
