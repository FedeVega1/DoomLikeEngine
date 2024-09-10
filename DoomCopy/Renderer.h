#pragma once

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

class Renderer
{
public:
	void RenderScreen(HWND hwnd);
	void InitRenderer();

private:
	std::unique_ptr<DWORD> buffer;
	void PaintScreen(DWORD color);
};
