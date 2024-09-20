#pragma once

#ifndef GDI_RENDER
#define GDI_RENDER
#endif

class GDIRenderer : public Renderer
{
public:
	virtual void RenderScreen(HWND hwnd) override;
	virtual HRESULT InitRenderer(HWND hwnd) override;
	virtual void DrawPixel(int x, int y, Color color) override;
	virtual void PaintScreen(Color color) override;
	virtual void ProcessGame(HWND hwnd, std::shared_ptr<Game> game) override;

	GDIRenderer();
	~GDIRenderer();

protected:
	virtual void DrawWall(const ProcessedWall& wall) override;
	virtual void ProcessSector(const ProcessedSector& sector) override;
	virtual ScreenSpaceWall GetScreenSpaceWall(const ProcessedWall& wall) override;

private:
	DWORD* buffer;
	BITMAPINFO bitmapInfo;

	HDC memHDC;
	HBITMAP hbmp;
};
