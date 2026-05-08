#pragma once

char appName[] = "Level Editor";
char appVersion[] = "0.0.1";
char appID[] = "com.Test.LevelEditor";

std::unique_ptr<Editor::EditorApp> editor;

SDL_WindowID wndID;
SDL_PropertiesID props;
SDL_Renderer* renderer;

inline constexpr int DEFAULT_WINDOW_WIDTH = 1280;
inline constexpr int DEFAULT_WINDOW_HEIGHT = 720;

bool InitializeSystem();
bool InitWindow();
bool InitImgGUI();
void MainLoop();
bool HandleEvents();
void ClearAndExit();
