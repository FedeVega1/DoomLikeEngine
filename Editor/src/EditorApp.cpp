#include "pch.h"

#include <imgui.h>
#include <SDL3/SDL.h>
#include <format>

#include "Editor/EditorTypes.h"
#include "Editor/MapView.h"
#include "Editor/BSPCompiler.h"
#include "Editor/MapFileIO.h"

#include "Editor/EditorApp.h"

namespace Editor
{
    EditorApp::EditorApp(const SDL_WindowID& wndID, SDL_Renderer* renderer) : window(window), renderer(renderer),
        mapView(std::make_unique<MapView>()), propertiesPanel(std::make_unique<Panels::PropertiesPanel>()),
        optionsPanel(std::make_unique<Panels::OptionsPanel>())
    {
    }

    EditorApp::~EditorApp() = default;

    void EditorApp::Update()
    {
        DrawMainMenuBar();
        DrawToolbar();
        DrawStatusBar();
        mapView->Update();
    }

    void EditorApp::Render()
    {
        if (ImGui::Begin("Map View")) mapView->Render();
        ImGui::End();

        if (ImGui::Begin("Properties")) propertiesPanel->Render(mapView->GetSelectedSector(), mapView->GetSelectedWall());
        ImGui::End();

        optionsPanel->Render();
    }

    void EditorApp::DrawMainMenuBar()
    {
        if (!ImGui::BeginMainMenuBar()) return;

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) NewMap();
            if (ImGui::MenuItem("Open...")) { /* TODO: file dialog */ }
            if (ImGui::MenuItem("Save")) { /* TODO */ }
            if (ImGui::MenuItem("Save As...")) { /* TODO */ }
            ImGui::Separator();

            if (ImGui::MenuItem("Compile Map...")) { /* TODO */ }
            ImGui::Separator();

            if (ImGui::MenuItem("Options")) optionsPanel->Toggle(true);
            ImGui::Separator();

            if (ImGui::MenuItem("Quit")) RequestQuit();
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* TODO */ }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* TODO */ }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    void EditorApp::DrawToolbar()
    {
        ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

        if (ImGui::Button("Line")) SetMode(EditorMode::Line);
        ImGui::SameLine();

        if (ImGui::Button("Node")) SetMode(EditorMode::Node);
        ImGui::SameLine();

        if (ImGui::Button("Wall")) SetMode(EditorMode::Wall);
        ImGui::SameLine();

        if (ImGui::Button("Sector")) SetMode(EditorMode::Sector);
        ImGui::End();
    }

    void EditorApp::DrawStatusBar()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos({ 0, io.DisplaySize.y - 24 });
        ImGui::SetNextWindowSize({ io.DisplaySize.x, 24 });
        ImGui::Begin("##statusbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize 
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

        const char* modeNames[] = { "None", "Line", "Node", "Wall", "Sector" };
        ImGui::Text("Mode: %s  |  %s", modeNames[static_cast<int>(currentMode)], currentFilePath.empty() ? "Untitled" : currentFilePath.c_str());

        ImGui::SameLine();

        Core::Vector2 mousePos = ImGui::GetIO().MousePos;
        float normalizedZoom = mapView->GetCurrentZoom() / 2.f;
        std::string gridData = "Zoom: " + std::format("{:.2f}", normalizedZoom) + " | Grid Size : " + std::to_string(mapView->GetCurrentGridSize());
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - ImGui::CalcTextSize(gridData.c_str()).x);
        ImGui::Text(gridData.c_str());

        ImGui::End();
    }

    void EditorApp::NewMap()
    {
        mapView->NewMap();
        currentFilePath.clear();
        unsavedChanges = false;
    }

    bool EditorApp::OpenMap(const std::string& filePath)
    {
        MapFileIO io;
        std::vector<EditorSector> sectors;

        if (!io.Read(filePath, sectors)) return false;
        mapView->LoadSectors(std::move(sectors));
        currentFilePath = filePath;
        unsavedChanges = false;
        return true;
    }

    bool EditorApp::SaveMap(const std::string& filePath)
    {
        MapFileIO io;
        if (!io.Write(filePath, mapView->GetSectors())) return false;

        currentFilePath = filePath;
        unsavedChanges = false;
        return true;
    }

    bool EditorApp::CompileAndSave(const std::string& bspFilePath)
    {
        BSPCompiler compiler;
        return compiler.Compile(mapView->GetSectors(), bspFilePath);
    }

    void EditorApp::SetMode(EditorMode mode) { currentMode = mode; }
}
