#include "pch.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "Editor/EditorTypes.h"
#include "Editor/MapView.h"
#include "Editor/EditorApp.h"
#include "Editor/Main.h"

int main(int argc, char* argv[])
{
    if (!InitializeSystem()) return 0;

    if (!SDL_ShowWindow(SDL_GetWindowFromID(wndID)))
    {
        OLOG_CF("There was an error while trying to show the app window: {0}", SDL_GetError());
        return 1;
    }

    MainLoop();

    ClearAndExit();
    return 0;
}

bool InitializeSystem()
{
    InitLogSystem(true, false);

    if (!SDL_SetAppMetadata(appName, appVersion, appID))
    {
        OLOG_CF("There was an error while setting up App Metadata: {0}", SDL_GetError());
        return false;
    }

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        OLOG_CF("There was an error while Initializing Video SubSytem: {0}", SDL_GetError());
        return false;
    }

    if (!InitWindow())
    {
        OLOG_CF("There was an error while setting up the main app window: {0}", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(SDL_GetWindowFromID(wndID), nullptr);

    if (!InitImgGUI())
    {
        OLOG_C("There was an error while setting up ImgGUI!");
        return false;
    }

    editor = std::make_unique<Editor::EditorApp>(wndID, renderer);
    return true;
}

bool InitWindow()
{
    props = SDL_CreateProperties();
    if (!props) return false;

    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, false);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, false);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, Editor::DEFAULT_WINDOW_WIDTH);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, Editor::DEFAULT_WINDOW_HEIGHT);

    SDL_Window* wnd = SDL_CreateWindowWithProperties(props);
    if (!wnd) return false;

    wndID = SDL_GetWindowID(wnd);
    return true;
}

bool InitImgGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.IniFilename = "editor_layout.ini";

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.AntiAliasedLines = true;
    style.AntiAliasedLinesUseTex = true;
    style.AntiAliasedFill = true;

    SDL_Window* window = SDL_GetWindowFromID(wndID);

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    return true;
}

void MainLoop()
{
    while (editor->IsRunning())
    {
        if (!HandleEvents()) break;

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        editor->Update();
        editor->Render();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }
}

bool HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);

        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                editor->RequestQuit();
                return false;
        }
    }

    return true;
}

void ClearAndExit()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);

    SDL_DestroyProperties(props);
    SDL_DestroyWindow(SDL_GetWindowFromID(wndID));

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}
