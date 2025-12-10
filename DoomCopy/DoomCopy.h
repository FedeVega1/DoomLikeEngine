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
bool clippedCursor;

BOOL SetupAndCreateWindow(const HINSTANCE hInstance, const int& nCmdShow);
LRESULT CALLBACK WndProc(const HWND const hWnd, const UINT message, const WPARAM const wParam, const LPARAM const lParam);
int MainLoop();
void SetupWindowMessages();
void GetClipRect();
