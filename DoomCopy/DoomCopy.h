#pragma once

#include "resource.h"
#include "Renderer.h"
//#include "D2Renderer.h"
#include "GDIRenderer.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

#ifdef GDI_RENDER
GDIRenderer renderer;
#endif

#ifdef D2_RENDER
D2Renderer renderer;
#endif


HWND mainHWND;
std::chrono::steady_clock::time_point startTime;
std::mt19937 rng;

BOOL SetupAndCreateWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int MainLoop();
long long GetGameTickCount();