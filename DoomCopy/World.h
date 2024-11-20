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

	Vector2 GetAvrgMiddlePoint() { return Vector2((leftPoint.x + rightPoint.x) / 2.0f, (leftPoint.y + rightPoint.y) / 2.0f); }
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

	bool HasPortals() const
	{ 
		for (int i = 0; i < numberOfWalls; i++)
		{
			if (sectorWalls[i].isPortal) 
				return true;
		}

		return false;
	}

	void GetPortalSectors(std::vector<int>* portalSectors, int ignoreSector) const
	{
		for (int i = 0; i < numberOfWalls; i++)
		{
			if (!sectorWalls[i].isPortal || sectorWalls[i].portalTargetSector == ignoreSector) continue;
			portalSectors->push_back(sectorWalls[i].portalTargetSector);
		}
	}

	float GetAvrgDistanceToPoint(Vector2 point) const
	{
		float avrgDistance = 0;
		for (int i = 0; i < numberOfWalls; i++)
			avrgDistance += Vector2::Distance(point, sectorWalls[i].GetAvrgMiddlePoint());

		return avrgDistance / numberOfWalls;
	}

	void GetMaxPoints(Vector2& min, Vector2& max) const
	{
		min = V2_ONE * 99999999.0f;
		max = V2_ONE * -99999999.0f;

		for (int i = 0; i < numberOfWalls; i++)
		{
			if (sectorWalls[i].leftPoint.x < min.x) min.x = sectorWalls[i].leftPoint.x;
			else if (sectorWalls[i].leftPoint.x > max.x) max.x = sectorWalls[i].leftPoint.x;

			if (sectorWalls[i].rightPoint.x < min.x) min.x = sectorWalls[i].rightPoint.x;
			else if (sectorWalls[i].rightPoint.x > max.x) max.x = sectorWalls[i].rightPoint.x;

			if (sectorWalls[i].leftPoint.y < min.y) min.y = sectorWalls[i].leftPoint.y;
			else if (sectorWalls[i].leftPoint.y > max.y) max.y = sectorWalls[i].leftPoint.y;

			if (sectorWalls[i].rightPoint.y < min.y) min.y = sectorWalls[i].rightPoint.y;
			else if (sectorWalls[i].rightPoint.y > max.y) max.y = sectorWalls[i].rightPoint.y;
		}
	}

	bool PointIsInsideSector(Vector2 point) const
	{
		Vector2 min, max;
		GetMaxPoints(min, max);
		return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
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
