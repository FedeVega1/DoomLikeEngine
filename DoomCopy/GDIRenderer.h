#pragma once

#ifndef GDI_RENDER
#define GDI_RENDER
#endif

typedef struct
{
	Vector2Int leftTopPoint, rightTopPoint, leftBtmPoint, rightBtmPoint;
	DWORD color;
} ScreenSpaceWall;


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
	DWORD* buffer;
	BITMAPINFO bitmapInfo;

	HDC memHDC;
	HBITMAP hbmp;

	void DrawWall(const ProcessedWall& wall);
	void ProcessSector(const ProcessedSector& sector);
	ScreenSpaceWall GetScreenSpaceWall(const ProcessedWall& wall);
};
