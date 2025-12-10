#pragma once
#include "RenderingDataTypes.h"

static inline int PixelPos(int x, int y) { return x + (DEFAULT_BUFFER_WIDTH * y); }

class Renderer
{
public:
	virtual void RenderScreen() = 0;
	virtual HRESULT InitRenderer(const HWND const hwnd) = 0;
	virtual void DrawPixel(int x, int y, const Color& color) = 0;
	virtual void PaintScreen(const Color& color) = 0;
	virtual void ProcessGame(Game* const game);

	Renderer() : debugStepDraw(false), screenBuffer(nullptr), drawBuffer(nullptr), hwnd(), spans(), brightnessDistanceFalloff(450.0f) { }

protected:
	HWND hwnd;

	DWORD* screenBuffer;
	DWORD* drawBuffer;
	bool debugStepDraw;

	float brightnessDistanceFalloff;

	std::vector<struct ProcessedWall> walls;
	std::vector<struct ScreenSpan> spans;

	std::map<std::wstring, BaseTexture> loadedTexturesMap;

	virtual void LoadTexture(const std::wstring& texName, const float& tilling = 1.0f) = 0;
	virtual void GetTextureMap(const std::wstring& texName, BaseTexture& texture) = 0;

	virtual void ProcessWall(const ProcessedWall& wall, Camera* const camera);
	virtual ScreenSpaceWall GetScreenSpaceWall(const ProcessedWall& wall);
	bool IsWallOccluded(Vector2Int wallSegment, SpanResult& result);
	ProcessedWall* GetProcessedWallPortalByID(const unsigned long long& wallID);
	void RenderPortalWall(const ProcessedWall& portal, const InPortalRenderData& data, OutPortalRenderData& outData);
	Color DarkenPixelColor(const Color& color, const float& value) const;
};
