#pragma once

#ifndef GDI_RENDER
#define GDI_RENDER
#endif

class GDIRenderer : public Renderer
{
public:
	virtual void RenderScreen(HWND hwnd, Game* const game) override;
	virtual HRESULT InitRenderer(HWND hwnd) override;
	virtual void DrawPixel(int x, int y, Color color) override;
	virtual void PaintScreen(Color color) override;
	virtual void ProcessGame(HWND hwnd, Game* const game) override;

	GDIRenderer();
	~GDIRenderer();

	void ToggleStepDraw(bool toggle) { debugStepDraw = toggle; }

protected:
	virtual void DrawWall(const ProcessedWall& wall, SectorSurface surface) override;
	virtual void DrawBackWall(const ProcessedWall& wall, SectorSurface surface, std::array<int, DEFAULT_BUFFER_WIDTH>& points) override;
	virtual void DrawSurfaces(const ProcessedWall& wall, SectorSurface surface, const std::array<int, DEFAULT_BUFFER_WIDTH>& points, Color ceilling, Color floor) override;
	virtual void ProcessSector(const ProcessedSector& sector) override;
	virtual ScreenSpaceWall GetScreenSpaceWall(const ProcessedWall& wall) override;

private:

	DWORD* buffer;
	BITMAPINFO bitmapInfo;

	bool debugStepDraw;
	HDC debugHDC;

	HDC memHDC;
	HBITMAP hbmp;
};
