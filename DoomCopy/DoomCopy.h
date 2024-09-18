#pragma once

#include "Game.h"
#include "Renderer.h"
//#include "D2Renderer.h"
#include "GDIRenderer.h"

#define IDM_EXIT 105

HINSTANCE hInst;

#ifdef GDI_RENDER
GDIRenderer renderer;
#endif

#ifdef D2_RENDER
D2Renderer renderer;
#endif

HWND mainHWND;
std::shared_ptr<Game> mainGame;
std::mt19937 rng;

BOOL SetupAndCreateWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int MainLoop();
