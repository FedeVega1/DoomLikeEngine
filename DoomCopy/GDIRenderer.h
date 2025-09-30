#pragma once

#ifndef GDI_RENDER
#define GDI_RENDER
#endif

class GDIRenderer : public Renderer
{
public:
	virtual void RenderScreen() override;
	virtual HRESULT InitRenderer(HWND hwnd) override;
	virtual void DrawPixel(int x, int y, Color color) override;
	virtual void PaintScreen(Color color) override;

	GDIRenderer();
	~GDIRenderer();

	void ToggleStepDraw(bool toggle) { debugStepDraw = toggle; }

private:
	BITMAPINFO bitmapInfo;

	HDC memHDC;
	HBITMAP hbmp;
};
