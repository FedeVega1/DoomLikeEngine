#pragma once

#include "Game.h"
#include "Renderer.h"
//#include "D2Renderer.h"
#include "GDIRenderer.h"

#define IDM_EXIT 105

typedef void (*WMessagePointer)(WPARAM, LPARAM);

wchar_t szWindowClass[] = L"DoomCopy";
wchar_t szTitle[] = L"DoomCopy Title";
HINSTANCE hInst;

#ifdef GDI_RENDER
GDIRenderer renderer;
#endif

#ifdef D2_RENDER
D2Renderer renderer;
#endif

HWND mainHWND;
Game mainGame;
RECT oldCursorClip;
RECT newCursorClip;
std::mt19937 rng;
std::map<UINT, WMessagePointer> messageMap;

BOOL SetupAndCreateWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int MainLoop();
void SetupWindowMessages();
void GetClipRect();
