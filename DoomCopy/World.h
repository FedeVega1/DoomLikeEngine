#pragma once

struct Wall
{
	Vector2 leftPoint, rightPoint;
	Color color;
	bool isPortal, isConnection;
	int portalTargetSector, portalTargetWall;

	Wall() : leftPoint(), rightPoint(), color(0, 0, 0), isPortal(false), isConnection(false), portalTargetSector(-1), portalTargetWall(-1)
	{ }

	Wall(Vector2 lPoint, Vector2 rPoint, Color c, bool portal, bool connection, int sector, int wall)
	{
		leftPoint = lPoint;
		rightPoint = rPoint;
		color = c;
		isPortal = portal;
		isConnection = connection;
		portalTargetSector = sector;
		portalTargetWall = wall;
	}
};

struct Sector
{ 
	Wall* sectorWalls;
	int numberOfWalls;
	float bottomPoint, topPoint;
	Color floorColor, ceillingColor;

	Sector() : sectorWalls(nullptr), numberOfWalls(0), bottomPoint(0.0f), topPoint(0.0f), floorColor(0, 0, 0), ceillingColor(0, 0, 0)
	{ }

	Sector(Wall* _sectorWalls, int _numberOfWalls, float _bottomPoint, float _topPoint, Color _floor, Color _ceilling)
	{ 
		sectorWalls = _sectorWalls;
		numberOfWalls = _numberOfWalls;
		bottomPoint = _bottomPoint;
		topPoint = _topPoint;
		floorColor = _floor;
		ceillingColor = _ceilling;
	}
};

class World : public Entity
{
public:
	Sector* sectorData;
	int numberOfSectors;

	World(const std::string& mapFileName);
	~World();

protected:
	virtual void Hack() override {}

private:
	const char BSPVersionSize = 2;
	char const BSPVersion[2]{ 00, 02 };

	int ByteArrayToInt(const unsigned char* const byteArray, bool isLittleEndian) const;
	Vector2Int ByteArrayToVector2Int(const unsigned char* const byteArray, bool isLittleEndian) const;
	Color ByteArrayToColor(const unsigned char* const byteArray) const;
};
