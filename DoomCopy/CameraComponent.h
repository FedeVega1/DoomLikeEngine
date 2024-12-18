#pragma once
class BaseComponent;

struct ProcessedWall
{
	Vector3 leftTopPoint, rightTopPoint, leftBtmPoint, rightBtmPoint;
	Color topColor, inColor, btmColor;
	bool isPortal, isConnection;
	int portalTargetSector, portalTargetWall;
	int wallSectorIndx;

	ProcessedWall() : leftTopPoint(), rightTopPoint(), leftBtmPoint(), rightBtmPoint(), topColor(0, 0, 0), 
		inColor(0, 0, 0), btmColor(0, 0, 0), isPortal(false), isConnection(false), portalTargetSector(-1), 
		portalTargetWall(-1), wallSectorIndx(-1)
	{ }

	ProcessedWall(Vector3 ltPoint, Vector3 rtPoint, Vector3 lbPoint, Vector3 rbPoint, Color tc, Color ic, Color bc,
		bool portal, bool connection, int sector, int wall, int sectorIndx)
	{ 
		leftTopPoint = ltPoint;
		rightTopPoint = rtPoint;
		leftBtmPoint = lbPoint;
		rightBtmPoint = rbPoint;
		topColor = tc;
		inColor = ic;
		btmColor = bc;
		isPortal = portal;
		isConnection = connection;
		portalTargetSector = sector;
		portalTargetWall = wall;
		wallSectorIndx = sectorIndx;
	}
};

struct ProcessedSector
{
	ProcessedWall* sectorWalls;
	int numberOfWalls, sectorID;
	float bottomPoint, topPoint;
	Color floorColor, ceillingColor;
	int worldSectorIndex;

	ProcessedSector() : sectorWalls(nullptr), numberOfWalls(0), bottomPoint(0.0f), topPoint(0.0f), 
		floorColor(0, 0, 0), ceillingColor(0, 0, 0), worldSectorIndex(-1), sectorID(-1)
	{ }
};

class Camera : public BaseComponent
{
	friend class Game;

public:
	virtual void Start() override;
	virtual void Tick() override;
	virtual void AfterTick() override;

	Camera() : BaseComponent(), xRotation(0), world(nullptr), processedSectors(nullptr), numbProcessedSectors(0), cameraZOffset(0), closestNodes(nullptr), numbProcessedWalls(0), processedWalls(nullptr)
	{ }

	~Camera()
	{
		for (int i = 0; i < 5; i++)
		{
			delete[] processedSectors.get()[i].sectorWalls;
			processedSectors.get()[i].sectorWalls = nullptr;
		}

		delete[] closestNodes;
		delete[] processedWalls;
	}

	 int GetProcessedWalls(ProcessedWall*& outProcessedWalls) const;
	 void SetCameraZOffset(float newOffset) { cameraZOffset = newOffset; }

protected:
	virtual void OnDestroy() override;

private:
	static const float movSpeed;
	static const float rotSpeed;

	int numbProcessedWalls;
	int numbProcessedSectors;
	int xRotation;
	float cameraZOffset;
	class World* world;
	std::shared_ptr<ProcessedSector[]> processedSectors;
	struct BSPNode** closestNodes;
	ProcessedWall* processedWalls;

	void ClipBehindCamera(Vector3& outPointA, const Vector3& pointB);
	void GetSectorsToProcess();
	void CopyWorldSectorData(int& sector, int worldSector);
	void ProcessWorldSector(int& sector, int worldSector, int portalIndx);
	bool CheckIfSectorIsProcessed(int sector);
	int GetClosestWorldSector();
	void GetWallsFromBSP(const Vector2& pos, struct BSPNode* startNode, struct BSPNode** closestNodesArray, int& outArraySize);

	void DebugLeftRight(float axis);
	void DebugForwardBack(float axis);
	void DebugUpDown(float axis);
	void DebugRotUpDown(float axis);
};
