#pragma once

#define DEFAULT_BUFFER_WIDTH 320
#define DEFAULT_BUFFER_HEIGHT 240

#define DEFAULT_WINDOW_WIDTH 1024
#define DEFAULT_WINDOW_HEIGHT 768

#define HALF_WIDTH DEFAULT_BUFFER_WIDTH / 2
#define HALF_HEIGHT DEFAULT_BUFFER_HEIGHT / 2

static inline int PixelPos(int x, int y) { return x + (DEFAULT_BUFFER_WIDTH * y); }

struct ScreenSpaceWall
{
	Vector2Int leftTopPoint, rightTopPoint, leftBtmPoint, rightBtmPoint;
	Color color;
};

class Renderer
{
public:
	virtual void RenderScreen(HWND hwnd, Game* const game) = 0;
	virtual HRESULT InitRenderer(HWND hwnd) = 0;
	virtual void DrawPixel(int x, int y, Color color) = 0;
	virtual void PaintScreen(Color color) = 0;
	virtual void ProcessGame(HWND hwnd, Game* const game);

	Renderer() : debugStepDraw(false) { }

protected:
	bool debugStepDraw;

	Vector3 GetWallNormal(Vector3 pointA, Vector3 pointB);
	virtual void ProcessSector(const ProcessedSector& sector);
	virtual ScreenSpaceWall GetScreenSpaceWall(const ProcessedWall& wall);
};
