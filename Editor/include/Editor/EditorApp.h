#pragma once

#include "Panels/PropertiesPanel.h"
#include "Panels/OptionsPanel.h"

namespace Editor
{
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
        std::unique_ptr<Panels::PropertiesPanel> propertiesPanel;
        std::unique_ptr<Panels::OptionsPanel> optionsPanel;
        CommandHistory history;

        EditorMode currentMode = EditorMode::None;
        std::string currentFilePath;
        bool unsavedChanges = false, running = true;
    };
}
