#pragma once

#include "Game.h"
#include "Renderer.h"
#include "SDLRenderer.h"

char appName[] = "DoomCopy";
char appVersion[] = "0.0.1";
char appID[] = "com.Test.DoomCopy";

Game mainGame;
SDLRenderer renderer;
SDL_WindowID wndID;
SDL_PropertiesID props;

void MainLoop();
bool InitWindow();
bool HandleEvents();
bool InitializeSystems();
void ClearAndExit();
