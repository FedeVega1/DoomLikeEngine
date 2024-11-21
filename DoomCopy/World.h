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

	Vector2 GetAvrgMiddlePoint() const { return Vector2((leftPoint.x + rightPoint.x) / 2.0f, (leftPoint.y + rightPoint.y) / 2.0f); }
};

struct Sector
{ 
	Wall* sectorWalls;
	int numberOfWalls, sectorID;
	float bottomPoint, topPoint;
	Color floorColor, ceillingColor;
	Vector2 sectorCenter;

	Sector() : sectorWalls(nullptr), numberOfWalls(0), bottomPoint(0.0f), topPoint(0.0f), floorColor(0, 0, 0), ceillingColor(0, 0, 0), sectorID(-1)
	{ }

	Sector(int id, Wall* _sectorWalls, int _numberOfWalls, float _bottomPoint, float _topPoint, Color _floor, Color _ceilling)
	{ 
		sectorID = id;
		sectorWalls = _sectorWalls;
		numberOfWalls = _numberOfWalls;
		bottomPoint = _bottomPoint;
		topPoint = _topPoint;
		floorColor = _floor;
		ceillingColor = _ceilling;
		sectorCenter = CalculateSectorCentroid();
	}

	bool HasPortals() const;
	void GetPortalSectors(std::vector<int>* portalSectors, int ignoreSector) const;
	float GetAvrgDistanceToPoint(Vector2 point) const;
	void GetMaxPoints(Vector2& min, Vector2& max) const;
	bool PointIsInsideSector(Vector2 point) const;
	Vector2 CalculateSectorCentroid() const;
};

class World : public Entity
{
public:
	friend class Game;
	friend class Camera;

	bool CheckIfPositionInsideSector(const Vector3& pos, int sector) const;
	bool CheckIfPositionInsideSector(const Vector3& pos, int* const sector) const;

	World(Game* const gameRef, const std::string& mapFileName);
	~World();

protected:
	virtual void Hack() override {}

private:
	Sector* sectorData;
	int numberOfSectors;

	const char BSPVersionSize = 2;
	char const BSPVersion[2]{ 00, 02 };

	int ByteArrayToInt(const unsigned char* const byteArray, bool isLittleEndian) const;
	Vector2Int ByteArrayToVector2Int(const unsigned char* const byteArray, bool isLittleEndian) const;
	Color ByteArrayToColor(const unsigned char* const byteArray) const;
};
