#pragma once

#ifndef GDI_RENDER
#define GDI_RENDER
#endif

class GDIRenderer : public Renderer
{
public:
	virtual void RenderScreen(HWND hwnd) override;
	virtual HRESULT InitRenderer(HWND hwnd) override;
	virtual void DrawPixel(int x, int y, DWORD color) override;
	virtual void PaintScreen(DWORD color) override;
	virtual void ProcessGame(HWND hwnd, std::shared_ptr<Game> game) override;

	GDIRenderer();
	~GDIRenderer();

private:
	std::shared_ptr<DWORD> buffer;
	BITMAPINFO bitmapInfo;

	HDC memHDC;
	HBITMAP hbmp;
};