#pragma once

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

#define DEFAULT_TICKS_PER_SECOND 30
#define SKIP_TICKS 1000 / DEFAULT_TICKS_PER_SECOND
#define MAX_FRAMESKIP 10

#define PIXELPOS(x, y) x * (y + 1)

class Renderer
{
public:
	void RenderScreen(HWND hwnd);
	void InitRenderer(HWND hwnd);
	void DrawPixel(int x, int y, DWORD color);

	~Renderer();

private:
	std::shared_ptr<DWORD> buffer;
	HDC memHDC;
	HBITMAP hbmp;

	void PaintScreen(DWORD color);
};
