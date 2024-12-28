#pragma once

struct Wall
{
	Vector2 leftPoint, rightPoint;
	Color topColor, inColor, btmColor;
	bool isPortal, isConnection;
	int portalTargetSector, portalTargetWall;
	unsigned long long wallID;

	Wall() : leftPoint(), rightPoint(), topColor(0, 0, 0), inColor(0, 0, 0), btmColor(0, 0, 0), isPortal(false), 
		isConnection(false), portalTargetSector(-1), portalTargetWall(-1), wallID(0ULL)
	{ }

	Wall(Vector2 lPoint, Vector2 rPoint) : topColor(0, 0, 0), inColor(0, 0, 0), btmColor(0, 0, 0), isPortal(false),
		isConnection(false), portalTargetSector(-1), portalTargetWall(-1), wallID(0ULL)
	{
		leftPoint = lPoint;
		rightPoint = rPoint;
	}

	Wall(Vector2 lPoint, Vector2 rPoint, Color tc, Color ic, Color bc, bool portal, bool connection, int sector, 
		int wall, unsigned long long id)
	{
		leftPoint = lPoint;
		rightPoint = rPoint;
		topColor = tc;
		inColor = ic;
		btmColor = bc;
		isPortal = portal;
		isConnection = connection;
		portalTargetSector = sector;
		portalTargetWall = wall;
		wallID = id;
	}

	Vector2 GetAvrgMiddlePoint() const { return Vector2((leftPoint.x + rightPoint.x) / 2.0f, (leftPoint.y + rightPoint.y) / 2.0f); }
	bool VectorInFrontWall(Vector2 vector) const;
};

struct Sector
{ 
	Wall* sectorWalls;
	int numberOfWalls, sectorID;
	float bottomPoint, topPoint;
	Color floorColor, ceillingColor;
	Vector2 sectorCenter;

	Sector() : sectorWalls(nullptr), numberOfWalls(0), bottomPoint(0.0f), topPoint(0.0f), floorColor(0, 0, 0), ceillingColor(0, 0, 0), sectorID(-1)
	{ }

	Sector(int id, Wall* _sectorWalls, int _numberOfWalls, float _bottomPoint, float _topPoint, Color _floor, Color _ceilling)
	{ 
		sectorID = id;
		sectorWalls = _sectorWalls;
		numberOfWalls = _numberOfWalls;
		bottomPoint = _bottomPoint;
		topPoint = _topPoint;
		floorColor = _floor;
		ceillingColor = _ceilling;
		sectorCenter = CalculateSectorCentroid();
	}

	bool HasPortals() const;
	void GetPortalSectors(std::vector<int>* portalSectors, int ignoreSector) const;
	float GetAvrgDistanceToPoint(Vector2 point) const;
	void GetMaxPoints(Vector2& min, Vector2& max) const;
	bool PointIsInsideSector(Vector2 point) const;
	Vector2 CalculateSectorCentroid() const;
};

struct BSPNode
{
	int sectorIndx;
	Wall wall;
	Wall splitter;
	BSPNode* frontNode;
	BSPNode* backNode;

	BSPNode() : sectorIndx(-1), wall(), splitter(), frontNode(nullptr), backNode(nullptr)
	{ }

	~BSPNode()
	{
		delete frontNode;
		delete backNode;
	}
};

class World : public Entity
{
public:
	friend class Game;
	friend class Camera;

	bool CheckIfPositionInsideSector(const Vector3& pos, int sector) const;
	bool CheckIfPositionInsideSector(const Vector3& pos, int* const sector) const;

	bool FindWallByID(unsigned long long id, int& wallIndx, int& sectorIndx) const;
	bool FindWallByIDWithSector(unsigned long long id, int sectorIndx, int& wallIndx) const;

	Sector GetSectorData(int sectorIndx) const { return sectorData[sectorIndx]; }

	World(Game* const gameRef, const std::string& mapFileName);
	~World();

protected:
	virtual void Hack() override {}

private:
	Sector* sectorData;
	int numberOfSectors;
	BSPNode rootNode;
	int maxNumberOfBSPNodes;

	static const int intSize = sizeof(int), pointSize = intSize * 2, colorSize = sizeof(char) * 3, idSize = sizeof(unsigned long long);
	unsigned char intBuffer[intSize], pointBuffer[pointSize], colorBuffer[colorSize], idBuffer[idSize];

	const char BSPVersionSize = 2;
	char const BSPVersion[2]{ 00, 05 };

	int ByteArrayToInt(const unsigned char* const byteArray, bool isLittleEndian) const;
	Vector2Int ByteArrayToVector2Int(const unsigned char* const byteArray, bool isLittleEndian) const;
	Color ByteArrayToColor(const unsigned char* const byteArray) const;
	unsigned long long ByteArrayToULL(const unsigned char* byteArray, bool isLittleEndian) const;
	void ReadBSPNode(BSPNode* currentNode, std::ifstream* stream, bool isLittleEndian, int& nodeCount) const;
};
