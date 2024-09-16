#pragma once

#define DEFAULT_BUFFER_WIDTH 800
#define DEFAULT_BUFFER_HEIGHT 600

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

#define DEFAULT_TICKS_PER_SECOND 30
#define SKIP_TICKS 1000 / DEFAULT_TICKS_PER_SECOND
#define MAX_FRAMESKIP 10

static inline int PixelPos(int x, int y) { return x + (DEFAULT_BUFFER_WIDTH * y); }

class Renderer
{
public:
	virtual void RenderScreen(HWND hwnd) = 0;
	virtual HRESULT InitRenderer(HWND hwnd) = 0;
	virtual void DrawPixel(int x, int y, DWORD color) = 0;
	virtual void PaintScreen(DWORD color) = 0;
};
