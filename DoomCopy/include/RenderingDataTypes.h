#pragma once

#define DEFAULT_BUFFER_WIDTH 320
#define DEFAULT_BUFFER_HEIGHT 240

#define DEFAULT_WINDOW_WIDTH 1024
#define DEFAULT_WINDOW_HEIGHT 768

#define HALF_WIDTH DEFAULT_BUFFER_WIDTH / 2
#define HALF_HEIGHT DEFAULT_BUFFER_HEIGHT / 2

inline Color ColorFromDWORD(DWORD v)
{
	return Color(static_cast<uint8_t>((v >> 16) & 0xFF), static_cast<uint8_t>((v >> 8) & 0xFF), static_cast<uint8_t>(v & 0xFF));
}

inline unsigned long ColorToDWORD(const Color& c)
{
	return 0xFF000000UL | (static_cast<unsigned long>(c.r) << 16) | (static_cast<unsigned long>(c.g) << 8) | static_cast<unsigned long>(c.b);
}

struct ScreenSpaceWall
{
	Vector2Int leftTopPoint, rightTopPoint, leftBtmPoint, rightBtmPoint;
	Color topColor, innerColor, bottomColor;

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

	ScreenSpan(const Vector2Int& segment, const bool& isConn)
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
	struct Camera* const cameraRef;
	struct BaseTexture* const textureRef;
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

	Color MapWallTexturePoint(const float& dWall, const int& relativeY, const float& wallHeight, const float& wallLength) const
	{
		if (!textureBuffer) return Color::White;

		float u = Wrap(dWall / width * tilling);
		float step = static_cast<float>(height) / wallHeight;

		Vector2Int tex = Vector2Int((int) std::floor(u * width), ((int) std::floor(relativeY * step)) & (height - 1));
		return ColorFromDWORD(textureBuffer[tex.x + (tex.y * width)]);
	}

	Color MapFloorCeilingTexturePoint(const Vector2& worldPos) const
	{
		if (!textureBuffer) return Color::White;

		float u = Wrap(-worldPos.x / width * tilling);
		float v = Wrap(-worldPos.y / height * tilling);

		Vector2Int tex = Vector2Int(static_cast<int>(std::floor(u * width)), static_cast<int>(std::floor(v * height)));
		return ColorFromDWORD(textureBuffer[tex.x + (tex.y * width)]);
	}

	float Wrap(const float& value) const { return value - std::floor(value); }
};
