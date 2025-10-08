#pragma once

#define DEFAULT_BUFFER_WIDTH 320
#define DEFAULT_BUFFER_HEIGHT 240

#define DEFAULT_WINDOW_WIDTH 1024
#define DEFAULT_WINDOW_HEIGHT 768

#define HALF_WIDTH DEFAULT_BUFFER_WIDTH / 2
#define HALF_HEIGHT DEFAULT_BUFFER_HEIGHT / 2

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

	ScreenSpan() : spanSegment(), isConnection(false), ceilPoints(), floorPoints() {}

	ScreenSpan(Vector2Int segment, bool isConn)
	{
		spanSegment = segment;
		isConnection = isConn;

		ceilPoints = { 0 };
		floorPoints = { DEFAULT_BUFFER_HEIGHT };
	}

	bool Intersects(Vector2Int otherSegment) const;
	bool ClampToSpan(Vector2Int& outOtherSegment) const;
};

struct SpanResult
{
	int connectionIndx;
	Vector2Int segment;

	SpanResult() : connectionIndx(-1), segment() {}
};

struct InPortalRenderData
{
	Vector2Int yPoint;
	int diff, dX, x;
	BYTE darkValue;
};


struct OutPortalRenderData
{
	int newCeillingY, newFloorY;
	bool hasDrawnC, hasDrawnF;

	OutPortalRenderData() : newCeillingY(0), newFloorY(0), hasDrawnC(false), hasDrawnF(false) {}
};

struct BaseTexture
{
	std::wstring textureName;
	int width, height;
	float tilling;
	DWORD* textureBuffer;

	Color MapTexturePoint(float dWall, int y, float yDiff, float wallLength) const
	{
		if (!textureBuffer) return COLOR_WHITE;

		float u = Wrap(dWall / (wallLength * tilling));

		float step = (float) height / yDiff;
		int texX = (int) std::floor(u * width);
		int texY = ((int) std::floor(y * step)) & (height - 1);

		return Color(textureBuffer[texX + (texY * width)], false);
	}

	int GetXPoint(const float& dx) const { return static_cast<int>(std::floor(dx * width)); }
	int GetYPoint(const float& dy) const { return static_cast<int>(std::floor(dy * height)); }

	float Wrap(float value) const { return value - std::floor(value); }
};
