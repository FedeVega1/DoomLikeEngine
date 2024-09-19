#pragma once
class BaseComponent;

struct ProcessedWall
{
	Vector3 leftTopPoint, rightTopPoint, leftBtmPoint, rightBtmPoint;
	DWORD color;
};

struct ProcessedSector
{
	ProcessedWall* sectorWalls;
	int numberOfWalls;
	float bottomPoint, topPoint, avrgDistanceToCamera;

	ProcessedSector() : sectorWalls(nullptr), numberOfWalls(0), bottomPoint(0.0f), topPoint(0.0f), avrgDistanceToCamera(0.0f)
	{ }
};

class Camera : public BaseComponent
{
	friend class Game;

public:
	virtual void Start() override;
	virtual void Tick() override;

	Camera() : BaseComponent(), world(nullptr), processedSectors(nullptr)
	{ }

	~Camera()
	{
		for (int i = 0; i < 5; i++)
			delete[] processedSectors[i].sectorWalls;

		delete[] processedSectors;
	}

	ProcessedSector* GetProcessedSectors();

protected:
	virtual void OnDestroy() override;

private:
	int xRotation;
	class World* world;
	ProcessedSector* processedSectors;

	void ClipBehindCamera(Vector3& pointA, const Vector3& pointB);
	int GetSectorsToProcess();

	void DebugForward();
	void DebugBack();
	void DebugLeft();
	void DebugRight();
	void DebugRotRight();
	void DebugRotLeft();
};
