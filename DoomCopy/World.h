#pragma once

struct Wall
{
	Vector2 leftPoint, rightPoint;
	DWORD color;

	Wall() : leftPoint(), rightPoint(), color(0)
	{ }

	Wall(Vector2 lPoint, Vector2 rPoint, DWORD c)
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
					Wall(Vector2(0, 0), Vector2(32, 0), 0xFF0000),
					Wall(Vector2(32, 0), Vector2(32, 32), 0xAA0000),
					Wall(Vector2(32, 32), Vector2(0, 32), 0xFF0000),
					Wall(Vector2(0, 32), Vector2(0, 0), 0xAA0000),
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
