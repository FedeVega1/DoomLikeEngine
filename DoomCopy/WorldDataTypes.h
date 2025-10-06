#pragma once

struct Wall
{
	Vector2 leftPoint, rightPoint;
	Color topColor, inColor, btmColor;
	bool isPortal, isConnection;
	unsigned long long wallID, portalWallTargetID;
	struct Sector* parentSector;
	struct Sector* portalTargetSector;
	Wall* portalTargetWall;

	Wall() : leftPoint(), rightPoint(), topColor(0, 0, 0), inColor(0, 0, 0), btmColor(0, 0, 0), isPortal(false),
		isConnection(false), wallID(0ULL), portalWallTargetID(0ULL), parentSector(nullptr), portalTargetSector(nullptr), portalTargetWall(nullptr)
	{
	}

	Wall(Vector2 lPoint, Vector2 rPoint) : topColor(0, 0, 0), inColor(0, 0, 0), btmColor(0, 0, 0), isPortal(false),
		isConnection(false), wallID(0ULL), portalWallTargetID(0ULL), parentSector(nullptr), portalTargetSector(nullptr), portalTargetWall(nullptr)
	{
		leftPoint = lPoint;
		rightPoint = rPoint;
	}

	Wall(Vector2 lPoint, Vector2 rPoint, Color tc, Color ic, Color bc, bool portal, bool connection, unsigned long long id, 
		unsigned long long wallTargetID, struct Sector* const sector, struct Sector* const portalSector, Wall* wallSector)
	{
		leftPoint = lPoint;
		rightPoint = rPoint;
		topColor = tc;
		inColor = ic;
		btmColor = bc;
		isPortal = portal;
		isConnection = connection;
		wallID = id;
		parentSector = sector;
		portalTargetSector = portalSector;
		portalTargetWall = wallSector;
		portalWallTargetID = wallTargetID;
	}

	Vector2 GetAvrgMiddlePoint() const { return Vector2((leftPoint.x + rightPoint.x) / 2.0f, (leftPoint.y + rightPoint.y) / 2.0f); }
	bool VectorInFrontWall(Vector2 vector) const;
};

struct Sector
{
	std::vector<Wall> sectorWalls;
	unsigned int sectorID;
	float bottomPoint, topPoint;
	Color floorColor, ceillingColor;
	Vector2 sectorCenter;

	Sector() : sectorWalls(), bottomPoint(0.0f), topPoint(0.0f), floorColor(0, 0, 0), ceillingColor(0, 0, 0), sectorID(0)
	{
	}

	Sector(unsigned int id, float _bottomPoint, float _topPoint, Color _floor, Color _ceilling) : sectorWalls()
	{
		sectorID = id;
		bottomPoint = _bottomPoint;
		topPoint = _topPoint;
		floorColor = _floor;
		ceillingColor = _ceilling;
		sectorCenter = CalculateSectorCentroid();
	}

	bool HasPortals() const;
	float GetAvrgDistanceToPoint(Vector2 point) const;
	void GetMaxPoints(Vector2& min, Vector2& max) const;
	bool PointIsInsideSector(Vector2 point) const;
	Vector2 CalculateSectorCentroid() const;
};

struct BoundingBox 
{
	Vector2Int topPoint, bottomPoint;

	BoundingBox() : topPoint(), bottomPoint()
	{
	}
};

struct Splitter
{
	Vector2Int startPoint, segment;
	Vector2 dir;

	Splitter() : startPoint(), segment(), dir()
	{
	}

	bool VectorInFront(Vector2 vector) const;
};

struct SubSector
{
	unsigned int subSectorID;
	std::vector<Wall> subSectorWalls;

	SubSector() : subSectorID(0)
	{
		subSectorWalls = std::vector<Wall>();
	}
};

class BSPNode
{
public:
	unsigned char childFlag;
	unsigned int nodeID, parentID, subSectorID;

	SubSector* subSector;
	Splitter splitter;

	BoundingBox bBox;

	BSPNode* frontNode;
	BSPNode* backNode;
	BSPNode* parentNode;

	BSPNode() : subSector(nullptr), splitter(), frontNode(nullptr), backNode(nullptr), parentNode(nullptr), nodeID(0), parentID(0), subSectorID(0), childFlag(0), bBox()
	{
	}

	~BSPNode()
	{
		delete frontNode;
		delete backNode;
	}

	bool IsLeafNode() const { return !frontNode && !backNode; }
};
