#pragma once

#include "resource.h"
#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
Renderer renderer;
HWND mainHWND;
std::chrono::steady_clock::time_point startTime;

BOOL SetupAndCreateWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int MainLoop();
long long GetGameTickCount();