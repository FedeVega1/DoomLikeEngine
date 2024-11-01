#pragma once
class BaseComponent;

enum SectorSurface { SurfNone, Above, Below };

struct ProcessedWall
{
	Vector3 leftTopPoint, rightTopPoint, leftBtmPoint, rightBtmPoint;
	Color color;

	ProcessedWall() : leftTopPoint(), rightTopPoint(), leftBtmPoint(), rightBtmPoint(), color(0, 0, 0)
	{ }

	ProcessedWall(Vector3 ltPoint, Vector3 rtPoint, Vector3 lbPoint, Vector3 rbPoint, Color c)
	{ 
		leftTopPoint = ltPoint;
		rightTopPoint = rtPoint;
		leftBtmPoint = lbPoint;
		rightBtmPoint = rbPoint;
		color = c;
	}
};

struct ProcessedSector
{
	ProcessedWall* sectorWalls;
	int numberOfWalls;
	float bottomPoint, topPoint, avrgDistanceToCamera;
	Color floorColor, ceillingColor;
	SectorSurface surface;

	ProcessedSector() : sectorWalls(nullptr), numberOfWalls(0), bottomPoint(0.0f), topPoint(0.0f), avrgDistanceToCamera(0.0f), floorColor(0, 0, 0), ceillingColor(0, 0, 0), surface(SectorSurface::SurfNone)
	{ }
};

class Camera : public BaseComponent
{
	friend class Game;

public:
	virtual void Start() override;
	virtual void Tick() override;

	Camera() : BaseComponent(), xRotation(0), world(nullptr), processedSectors(nullptr), numbProcessedSectors(0), cameraZOffset(0)
	{ }

	~Camera()
	{
		for (int i = 0; i < 5; i++)
		{
			delete[] processedSectors.get()[i].sectorWalls;
			processedSectors.get()[i].sectorWalls = nullptr;
		}
	}

	 int GetProcessedSectors(std::shared_ptr<ProcessedSector[]> & outProcessedSectors);
	 void SetCameraZOffset(float newOffset) { cameraZOffset = newOffset; }

protected:
	virtual void OnDestroy() override;

private:
	static const float movSpeed;
	static const float rotSpeed;
	static const int numberOfSectorsToProcess = 5;

	int numbProcessedSectors;
	int xRotation;
	float cameraZOffset;
	class World* world;
	std::shared_ptr<ProcessedSector[]> processedSectors;

	void ClipBehindCamera(Vector3& pointA, const Vector3& pointB);
	int GetSectorsToProcess();
	void OrderSectorsByDistance();

	void DebugLeftRight(float axis);
	void DebugForwardBack(float axis);
	void DebugUpDown(float axis);
	void DebugRotUpDown(float axis);
};
