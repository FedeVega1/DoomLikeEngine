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
	Color topColor, inColor, btmColor;

	Vector2Int GetSegment() const
	{ 
		int start = std::clamp(leftBtmPoint.x, 0, DEFAULT_BUFFER_WIDTH);
		int end = std::clamp(rightBtmPoint.x, 0, DEFAULT_BUFFER_WIDTH);
		return Vector2Int(start, end); 
	}
};

struct ScreenSpan
{
	Vector2Int spanSegment;
	bool isConnection;
	std::array<int, DEFAULT_BUFFER_WIDTH> ceilPoints;
	std::array<int, DEFAULT_BUFFER_WIDTH> floorPoints;

	ScreenSpan() : spanSegment(), isConnection(false), ceilPoints(), floorPoints() { }

	ScreenSpan(Vector2Int segment, bool isConn)
	{
		spanSegment = segment;
		isConnection = isConn;

		ceilPoints = {0};
		floorPoints = {DEFAULT_BUFFER_HEIGHT};
	}

	bool Intersects(Vector2Int otherSegment) const;
	bool ClampToSpan(Vector2Int& outOtherSegment) const;
};

struct SpanResult
{
	int connectionIndx;
	Vector2Int segment;

	SpanResult() : connectionIndx(-1), segment() { }
};

struct InPortalRenderData
{
	Vector2Int yPoint;
	int diff, dX, x;
};


struct OutPortalRenderData
{
	int newCeillingY, newFloorY;
	bool hasDrawnC, hasDrawnF;

	OutPortalRenderData() : newCeillingY(0), newFloorY(0), hasDrawnC(false), hasDrawnF(false) { }
};

class Renderer
{
public:
	virtual void RenderScreen() = 0;
	virtual HRESULT InitRenderer(HWND hwnd) = 0;
	virtual void DrawPixel(int x, int y, Color color) = 0;
	virtual void PaintScreen(Color color) = 0;
	virtual void ProcessGame(Game* const game);

	Renderer() : debugStepDraw(false), screenBuffer(nullptr), drawBuffer(nullptr), hwnd(), spans() { }

protected:
	HWND hwnd;

	DWORD* screenBuffer;
	DWORD* drawBuffer;
	bool debugStepDraw;

	std::vector<struct ProcessedWall> walls;
	std::vector<struct ScreenSpan> spans;

	Vector3 GetWallNormal(Vector3 pointA, Vector3 pointB);
	virtual void ProcessWall(const ProcessedWall& wall);
	virtual ScreenSpaceWall GetScreenSpaceWall(const ProcessedWall& wall);
	bool IsWallOccluded(Vector2Int wallSegment, SpanResult& result);
	ProcessedWall* GetProcessedWallPortalByID(unsigned long long wallID);
	void RenderPortalWall(const ProcessedWall& portal, const InPortalRenderData& data, OutPortalRenderData& outData);
};
