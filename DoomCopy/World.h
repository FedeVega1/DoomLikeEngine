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

	World() : Entity()
	{
		numberOfSectors = 2;
		sectorData = new Sector[numberOfSectors]
		{
			Sector(
				new Wall[4]
				{
					Wall(V2_ZERO, Vector2(32, 0), COLOR_RED),
					Wall(Vector2(32, 0), Vector2(32, 32), COLOR_DARKRED),
					Wall(Vector2(32, 32), Vector2(0, 32), COLOR_RED),
					Wall(Vector2(0, 32), V2_ZERO, COLOR_DARKRED),
				},
				4,
				0.0f,
				40.0f,
				COLOR_GREEN, 
				COLOR_DARKGREEN
			),
			Sector(
				new Wall[4]
				{
					Wall(Vector2(48, 0), Vector2(80, 48), COLOR_BLUE),
					Wall(Vector2(80, 48), Vector2(80, 80), COLOR_DARKBLUE),
					Wall(Vector2(80, 80), Vector2(48, 80), COLOR_BLUE),
					Wall(Vector2(48, 80), Vector2(48, 0), COLOR_DARKBLUE),
				},
				4,
				0.0f,
				40.0f,
				COLOR_GREEN, 
				COLOR_DARKGREEN
			)
		};
	}

	~World()
	{
		Entity::~Entity();

		for (int i = 0; i < numberOfSectors; i++)
			delete[] sectorData[i].sectorWalls;

		delete[] sectorData;
	}

protected:
	virtual void Hack() override {}
};
