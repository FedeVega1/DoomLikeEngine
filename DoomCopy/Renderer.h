#pragma once
#include "RenderingDataTypes.h"

static inline int PixelPos(int x, int y) { return x + (DEFAULT_BUFFER_WIDTH * y); }

class Renderer
{
public:
	virtual void RenderScreen() = 0;
	virtual HRESULT InitRenderer(HWND hwnd) = 0;
	virtual void DrawPixel(int x, int y, Color color) = 0;
	virtual void PaintScreen(Color color) = 0;
	virtual void ProcessGame(Game* const game);

	Renderer() : debugStepDraw(false), screenBuffer(nullptr), drawBuffer(nullptr), hwnd(), spans(), darkStep(200), minDarkValue(5) { }

protected:
	HWND hwnd;

	DWORD* screenBuffer;
	DWORD* drawBuffer;
	bool debugStepDraw;

	BYTE darkStep, minDarkValue;

	std::vector<struct ProcessedWall> walls;
	std::vector<struct ScreenSpan> spans;

	std::map<std::wstring, BaseTexture> loadedTexturesMap;

	virtual void LoadTexture(const std::wstring& texName) = 0;
	virtual void GetTextureMap(const std::wstring& texName, BaseTexture& texture) = 0;

	Vector3 GetWallNormal(Vector3 pointA, Vector3 pointB);
	virtual void ProcessWall(const ProcessedWall& wall, Camera* const camera);
	virtual ScreenSpaceWall GetScreenSpaceWall(const ProcessedWall& wall);
	bool IsWallOccluded(Vector2Int wallSegment, SpanResult& result);
	ProcessedWall* GetProcessedWallPortalByID(unsigned long long wallID);
	void RenderPortalWall(const ProcessedWall& portal, const InPortalRenderData& data, OutPortalRenderData& outData);
	Color DarkenPixelColor(const Color& color, const BYTE& value) const;
};
