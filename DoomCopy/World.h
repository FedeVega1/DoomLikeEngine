#pragma once

struct Wall
{
	Vector2 leftPoint, rightPoint;
	Color color;

	Wall() : leftPoint(), rightPoint(), color(0, 0, 0)
	{ }

	Wall(Vector2 lPoint, Vector2 rPoint, Color c)
	{
		leftPoint = lPoint;
		rightPoint = rPoint;
		color = c;
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
	int ByteArrayToInt(const char* byteArray);
	Vector2Int ByteArrayToVector2Int(const char* byteArray);
	Color ByteArrayToColor(const char* byteArray);
};
