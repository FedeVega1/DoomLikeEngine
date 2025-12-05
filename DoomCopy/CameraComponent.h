#pragma once
class BaseComponent;

struct ProcessedWall
{
	Vector3 leftTopPoint, rightTopPoint, leftBtmPoint, rightBtmPoint;
	Color topColor, inColor, btmColor;
	bool isPortal, isConnection;
	struct Sector* parentSector;
	struct Sector* portalTargetSector;
	struct Wall* portalTargetWall;
	const struct Wall* referenceWall;

	ProcessedWall() : leftTopPoint(), rightTopPoint(), leftBtmPoint(), rightBtmPoint(), topColor(0, 0, 0), 
		inColor(0, 0, 0), btmColor(0, 0, 0), isPortal(false), isConnection(false), portalTargetSector(nullptr), 
		portalTargetWall(nullptr), parentSector(nullptr), referenceWall(nullptr)
	{ }

	ProcessedWall(Vector3 ltPoint, Vector3 rtPoint, Vector3 lbPoint, Vector3 rbPoint, Color tc, Color ic, Color bc,
		bool portal, bool connection, struct Sector* const targetSector, struct Wall* const wall, struct Sector* const sector, const struct Wall* const refWall)
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
		portalTargetSector = targetSector;
		portalTargetWall = wall;
		parentSector = sector;
		referenceWall = refWall;
	}

	Vector2 FromScreenToWorldSpace(float interp) const;
};

class Camera : public BaseComponent
{
	friend class Game;

public:
	virtual void Start() override;
	virtual void Tick() override;
	virtual void AfterTick() override;

	Camera() : BaseComponent(), xRotation(0), world(nullptr), cameraZOffset(0), processedWalls(), toggleBSPRendering(false)
	{ }

	~Camera()
	{
	}

	 void GetProcessedWalls(std::vector<ProcessedWall>& outProcessedWalls) const;
	 void SetCameraZOffset(float newOffset) { cameraZOffset = newOffset; }
	 Vector2 GetWorldPointFromRay(int screenX, int screenWidth, const ProcessedWall& wall);
	 Vector2 GetFloorCeilingHitPoint(const Vector2& normalizedScreenCoords, const float& planeHeight);

protected:
	virtual void OnDestroy() override;

private:
	static const float movSpeed;
	static const float rotSpeed;

	bool toggleBSPRendering;
	int xRotation;
	float cameraZOffset;
	class World* world;
	std::vector<ProcessedWall> processedWalls;

	void ClipBehindCamera(Vector3& outPointA, const Vector3& pointB);
	
	void GetWallsFromBSP(const Vector3& pos, class BSPNode* startNode, const double& cos, const double& sin);
	void ProcessSubSectorFromBSPNode(const struct SubSector* const subSector, Vector3 pos, const double& cos, const double& sin);
	void RenderAllSubSectors(Vector3 pos, const double& cos, const double& sin);
	void RenderWall(ProcessedWall& wall, Vector3 pos, const double& cos, const double& sin);

	void DebugLeftRight(float axis);
	void DebugForwardBack(float axis);
	void DebugUpDown(float axis);
	void DebugRotUpDown(float axis);
	void DebugToggleBSPRendering();
};
