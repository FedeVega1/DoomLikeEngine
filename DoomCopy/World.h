#pragma once

typedef struct
{
	Vector3 leftTopPoint, rightTopPoint, leftBtmPoint, rightBtmPoint;
} Wall;

class World : public Entity
{
public:
	Wall wallData;
	Wall wall;

	World() : Entity()
	{
		wallData = Wall { Vector3(40, 40, 10), Vector3(40, 40, 290), Vector3(40, 0, 10), Vector3(40, 0, 290) };
		wall = Wall { wallData.leftTopPoint, wallData.rightTopPoint, wallData.leftBtmPoint, wallData.rightBtmPoint };
	}

protected:
	virtual void Hack() override {}
};
