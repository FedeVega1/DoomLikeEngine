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

protected:
	virtual void LoadTexture(const std::wstring& texName) override;
	virtual void GetTextureMap(const std::wstring& texName, BaseTexture& texture) override;

private:
	BITMAPINFO bitmapInfo;

	HDC memHDC;
	HBITMAP hbmp;
};
