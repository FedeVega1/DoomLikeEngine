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

	Sector() : sectorWalls(nullptr), numberOfWalls(0), bottomPoint(0.0f), topPoint(0.0f)
	{ }

	Sector(Wall* _sectorWalls, int _numberOfWalls, float _bottomPoint, float _topPoint)
	{ 
		sectorWalls = _sectorWalls;
		numberOfWalls = _numberOfWalls;
		bottomPoint = _bottomPoint;
		topPoint = _topPoint;
	}
};

class World : public Entity
{
public:
	Sector* sectorData;
	int numberOfSectors;

	World() : Entity()
	{
		numberOfSectors = 1;
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
				40.0f
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
