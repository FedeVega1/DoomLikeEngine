#pragma once
class BaseComponent;

struct ProcessedWall
{
	Vector3 leftTopPoint, rightTopPoint, leftBtmPoint, rightBtmPoint;
	Color topColor, innerColor, bottomColor;
	bool isPortal, isConnection;
	const Sector* parentSector;
	const Sector* portalTargetSector;
	Wall* portalTargetWall;
	const Wall* referenceWall;

	ProcessedWall() : leftTopPoint(), rightTopPoint(), leftBtmPoint(), rightBtmPoint(), topColor(0, 0, 0),
		innerColor(0, 0, 0), bottomColor(0, 0, 0), isPortal(false), isConnection(false), portalTargetSector(nullptr),
		portalTargetWall(nullptr), parentSector(nullptr), referenceWall(nullptr)
	{ }

	ProcessedWall(Vector3 ltPoint, Vector3 rtPoint, Vector3 lbPoint, Vector3 rbPoint, Color tc, Color ic, Color bc,
		bool portal, bool connection, const Sector* targetSector, Wall* wall, const Sector* sector, const Wall* refWall)
	{
		leftTopPoint = ltPoint;
		rightTopPoint = rtPoint;
		leftBtmPoint = lbPoint;
		rightBtmPoint = rbPoint;
		topColor = tc;
		innerColor = ic;
		bottomColor = bc;
		isPortal = portal;
		isConnection = connection;
		portalTargetSector = targetSector;
		portalTargetWall = wall;
		parentSector = sector;
		referenceWall = refWall;
	}
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
	 void SetCameraZOffset(const float& newOffset) { cameraZOffset = newOffset; }
	 Vector2 GetWorldPointFromRay(const int& screenX, const int& screenWidth, const ProcessedWall& wall);
	 Vector2 GetFloorCeilingHitPoint(const Vector2& normalizedScreenCoords, const float& planeHeight);
	 float GetDistanceToPoint(const Vector2& worldPoint);

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
	
	void GetWallsFromBSP(const Vector3& pos, BSPNode* const startNode, const double& cos, const double& sin);
	void ProcessSubSectorFromBSPNode(const struct SubSector* const subSector, Vector3 pos, const double& cos, const double& sin);
	void RenderAllSubSectors(Vector3 pos, const double& cos, const double& sin);
	void RenderWall(ProcessedWall& wall, const Vector3& pos, const double& cos, const double& sin);

	void DebugLeftRight(float axis);
	void DebugForwardBack(float axis);
	void DebugUpDown(float axis);
	void DebugRotUpDown(float axis);
	void DebugToggleBSPRendering();
};
