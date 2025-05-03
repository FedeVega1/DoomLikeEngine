#include "pch.h"
#include "VectorMath.h"
#include "GameObjects.h"
#include "World.h"
#include <fstream>
#include <filesystem>
#include <bit>

int front = 0, back = 0;

int Partition(BSPNode** const arr, int left, int right)
{
	int pivot = arr[left]->nodeID;
	while (true)
	{
		while (arr[left]->nodeID < pivot) left++;
		while (arr[right]->nodeID > pivot) right--;

		if (left < right)
		{
			if (arr[left]->nodeID == arr[right]->nodeID) return right;

			BSPNode* temp = arr[left];
			arr[left] = arr[right];
			arr[right] = temp;
			continue;
		}

		return right;
	}
}

void Quick_Sort(BSPNode** arr, int left, int right)
{
	if (left < right)
	{
		int pivot = Partition(arr, left, right);

		if (pivot > 1) Quick_Sort(arr, left, pivot - 1);
		if (pivot + 1 < right) Quick_Sort(arr, pivot + 1, right);
	}
}

World::World(Game* const gameRef, const std::string& mapFileName) : Entity(gameRef), numberOfSectors(0), sectorData(nullptr), maxNumberOfBSPNodes(0)
{
	OLOG_LF("Working Directory: {0}", std::filesystem::current_path().string());

	std::ifstream mapFile;
	mapFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try 
	{
		mapFile.open(mapFileName, std::ios_base::in | std::ios_base::binary);
		OLOG_LF("Reading Map: {0}", mapFileName);

		mapFile.read((char*) intBuffer, intSize);
		bool isLittleEndian = intBuffer[0] == 0x1;

		unsigned char versionSize;
		mapFile.read((char*) &versionSize, 1);

		if (versionSize != BSPVersionSize)
		{
			OLOG_EF("Bad BSP version size. Expected {0}, got {1}", BSPVersionSize, versionSize);
			return;
		}

		unsigned char* versionArray = new unsigned char[versionSize];
		mapFile.read((char*) versionArray, versionSize);

		for (int i = 0; i < versionSize; i++)
		{
			if (BSPVersion[i] == versionArray[i]) continue;
			OLOG_E("Bad BSP version number!");
			return;
		}

		delete[] versionArray;
		OLOG_L("BSP version Correct!");

		mapFile.read((char*) intBuffer, intSize);
		numberOfSectors = ByteArrayToInt(intBuffer, isLittleEndian);
		OLOG_LF("Number of Sectors: {0}", numberOfSectors);

		sectorData = new Sector[numberOfSectors];
		for (int i = 0; i < numberOfSectors; i++)
		{
			Sector& sector = sectorData[i] = Sector();

			mapFile.read((char*)intBuffer, intSize);
			sector.sectorID = ByteArrayToUInt(intBuffer, isLittleEndian);

			OLOG_LF("Loading Sector: {0}", sector.sectorID);

			mapFile.read((char*) intBuffer, intSize);
			sector.topPoint = static_cast<float>(ByteArrayToInt(intBuffer, isLittleEndian));

			mapFile.read((char*) intBuffer, intSize);
			sector.bottomPoint = static_cast<float>(ByteArrayToInt(intBuffer, isLittleEndian));

			mapFile.read((char*) colorBuffer, colorSize);
			sector.floorColor = ByteArrayToColor(colorBuffer);

			mapFile.read((char*) colorBuffer, colorSize);
			sector.ceillingColor = ByteArrayToColor(colorBuffer);

			OLOG_LF("Loaded Sector: {0} Correct", sector.sectorID);
		}

		mapFile.read((char*) intBuffer, intSize);

		mapFile.read((char*) intBuffer, intSize);
		maxNumberOfBSPNodes = ByteArrayToInt(intBuffer, isLittleEndian);

		ReadBSPNode(&mapFile, isLittleEndian);

		//OLOG_LF("Loaded {0} BSP nodes from a total of {1}", nodeCount, maxNumberOfBSPNodes);
		//OLOG_LF("Loaded {0} front Nodes and {1} back nodes", front, back);

		mapFile.close();
		OLOG_L("Loaded all Sectors Correct!");
	}
	catch (std::system_error& e)
	{
		char buffer[50];
		strerror_s(buffer, 50, errno);
		OLOG_CF("Couldn't load file {0}!! - Error: {1}, {2}", mapFileName, e.code().message(), buffer);
	}
}

void World::ReadBSPNode(std::ifstream* stream, bool isLittleEndian)
{
	unsigned char singleByte;
	BSPNode** nodeArray = new BSPNode*[maxNumberOfBSPNodes];

	for (int i = 0; i < maxNumberOfBSPNodes; i++)
	{
		BSPNode& node = *(nodeArray[i] = new BSPNode());

		stream->read((char*) &singleByte, 1);
		node.childFlag = singleByte;

		stream->read((char*) intBuffer, intSize);
		node.nodeID = ByteArrayToUInt(intBuffer, isLittleEndian);

		Wall newWall;
		GetWallFromFile(stream, isLittleEndian, newWall);
		node.wall = newWall;

		Wall splitter = Wall();

		stream->read((char*) pointBuffer, pointSize);
		splitter.leftPoint = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

		stream->read((char*) pointBuffer, pointSize);
		splitter.rightPoint = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

		stream->read((char*) idBuffer, idSize);
		splitter.wallID = ByteArrayToULL(idBuffer, isLittleEndian);

		node.splitter = splitter;

		stream->read((char*) intBuffer, intSize);
		node.parentID = ByteArrayToUInt(intBuffer, isLittleEndian);

		OLOG_LF("Loaded Node {0} with parentID {1} and childFlag {2}", node.nodeID, node.parentID, node.childFlag);
	}

	Quick_Sort(nodeArray, 0, maxNumberOfBSPNodes - 1);

	for (int i = 0; i < maxNumberOfBSPNodes; i++)
	{
		if (nodeArray[i]->childFlag == 0xFF)
		{
			rootNode = nodeArray[i];
			continue;
		}

		if (nodeArray[i]->parentID != 0xFFFFFFFF)
			nodeArray[i]->parentNode = nodeArray[nodeArray[i]->parentID];

		if (nodeArray[i]->childFlag == 0xBB)
		{
			nodeArray[nodeArray[i]->parentID]->frontNode = nodeArray[i];
			continue;
		}

		if (nodeArray[i]->childFlag != 0xCC) continue;
		nodeArray[nodeArray[i]->parentID]->backNode = nodeArray[i];
	}

	delete[] nodeArray;
}

void World::GetWallFromFile(std::ifstream* stream, bool isLittleEndian, Wall& wall) const
{
	wall = Wall();

	stream->read((char*) pointBuffer, pointSize);
	wall.leftPoint = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

	stream->read((char*) pointBuffer, pointSize);
	wall.rightPoint = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

	stream->read((char*) colorBuffer, colorSize);
	wall.topColor = ByteArrayToColor(colorBuffer);

	stream->read((char*) colorBuffer, colorSize);
	wall.inColor = ByteArrayToColor(colorBuffer);

	stream->read((char*) colorBuffer, colorSize);
	wall.btmColor = ByteArrayToColor(colorBuffer);

	unsigned char portalFlags;
	stream->read((char*) &portalFlags, 1);

	wall.isPortal = (portalFlags & 0b1) == 0b1;
	wall.isConnection = (portalFlags & 0b10) == 0b10;

	stream->read((char*) intBuffer, intSize);
	wall.portalTargetSector = ByteArrayToInt(intBuffer, isLittleEndian);

	stream->read((char*) intBuffer, intSize);
	wall.portalTargetWall = ByteArrayToInt(intBuffer, isLittleEndian);

	stream->read((char*) idBuffer, idSize);
	wall.wallID = ByteArrayToULL(idBuffer, isLittleEndian);

	stream->read((char*) intBuffer, intSize);
	unsigned int sectorID = ByteArrayToUInt(intBuffer, isLittleEndian);

	for (int i = 0; i < numberOfSectors; i++)
	{
		if (sectorID != sectorData[i].sectorID) continue;
		wall.parentSector = &sectorData[i];
		sectorData[i].sectorWalls.push_back(&wall);
		return;
	}

	OLOG_CF("Couldn't find sector ID {0}", sectorID);
	throw std::system_error(1, std::generic_category());
}

World::~World()
{
	Entity::~Entity();

	//for (int i = 0; i < numberOfSectors; i++)
	//{
	//	delete[] sectorData[i].sectorWalls;
	//	sectorData[i].sectorWalls = nullptr;
	//}

	delete[] sectorData;
	sectorData = nullptr;
}

int World::ByteArrayToInt(const unsigned char* const byteArray, bool isLittleEndian) const
{
	if (isLittleEndian) return (static_cast<int>(byteArray[3]) << 24) | (static_cast<int>(byteArray[2]) << 16) | (static_cast<int>(byteArray[1]) << 8) | static_cast<int>(byteArray[0]);
	return (static_cast<int>(byteArray[0]) << 24) | (static_cast<int>(byteArray[1]) << 16) | (static_cast<int>(byteArray[2]) << 8) | static_cast<int>(byteArray[3]);
}

unsigned int World::ByteArrayToUInt(const unsigned char* const byteArray, bool isLittleEndian) const
{
	if (isLittleEndian) return (static_cast<unsigned int>(byteArray[3]) << 24) | (static_cast<unsigned int>(byteArray[2]) << 16) | (static_cast<unsigned int>(byteArray[1]) << 8) | static_cast<unsigned int>(byteArray[0]);
	return (static_cast<unsigned int>(byteArray[0]) << 24) | (static_cast<unsigned int>(byteArray[1]) << 16) | (static_cast<unsigned int>(byteArray[2]) << 8) | static_cast<unsigned int>(byteArray[3]);
}

unsigned long long World::ByteArrayToULL(const unsigned char* byteArray, bool isLittleEndian) const
{
	if (isLittleEndian)
	{
		int high = (static_cast<int>(byteArray[7]) << 24) | (static_cast<int>(byteArray[6]) << 16) | (static_cast<int>(byteArray[5]) << 8) | static_cast<int>(byteArray[4]);
		int low = (static_cast<int>(byteArray[3]) << 24) | (static_cast<int>(byteArray[2]) << 16) | (static_cast<int>(byteArray[1]) << 8) | static_cast<int>(byteArray[0]);
		return ((unsigned long long) high) | ((unsigned long long) low);
	}

	int high = (static_cast<int>(byteArray[0]) << 24) | (static_cast<int>(byteArray[1]) << 16) | (static_cast<int>(byteArray[2]) << 8) | static_cast<int>(byteArray[3]);
	int low = (static_cast<int>(byteArray[4]) << 24) | (static_cast<int>(byteArray[5]) << 16) | (static_cast<int>(byteArray[6]) << 8) | static_cast<int>(byteArray[7]);
	return ((unsigned long long) high) | ((unsigned long long) low);
}

Vector2Int World::ByteArrayToVector2Int(const unsigned char* const byteArray, bool isLittleEndian) const
{
	const int intSize = sizeof(int);
	unsigned char x[intSize] { byteArray[0], byteArray[1], byteArray[2], byteArray[3] };
	unsigned char y[intSize] { byteArray[4], byteArray[5], byteArray[6], byteArray[7] };
	return Vector2Int(ByteArrayToInt(x, isLittleEndian), ByteArrayToInt(y, isLittleEndian));
}

Color World::ByteArrayToColor(const unsigned char* const byteArray) const { return Color(byteArray[0], byteArray[1], byteArray[2]); }

bool World::CheckIfPositionInsideSector(const Vector3& pos, int sector) const
{
	return sectorData[sector].PointIsInsideSector(pos.XY());
}

bool World::CheckIfPositionInsideSector(const Vector3& pos, int* const sector) const
{
	*sector = -1;

	for (int i = 0; i < numberOfSectors; i++)
	{
		if (!sectorData[i].PointIsInsideSector(pos.XY())) continue;
		*sector = i;
		return true;
	}

	return false;
}

bool Sector::HasPortals() const
{
	for (size_t i = 0; i < sectorWalls.size(); i++)
	{
		if (sectorWalls[i]->isPortal)
			return true;
	}

	return false;
}

void Sector::GetPortalSectors(std::vector<int>*portalSectors, int ignoreSector) const
{
	for (size_t i = 0; i < sectorWalls.size(); i++)
	{
		if (!sectorWalls[i]->isPortal || sectorWalls[i]->portalTargetSector == ignoreSector) continue;
		portalSectors->push_back(sectorWalls[i]->portalTargetSector);
	}
}

float Sector::GetAvrgDistanceToPoint(Vector2 point) const
{
	float avrgDistance = 0;
	for (int i = 0; i < sectorWalls.size(); i++)
		avrgDistance += Vector2::Distance(point, sectorWalls[i]->GetAvrgMiddlePoint());

	return avrgDistance / sectorWalls.size();
}

void Sector::GetMaxPoints(Vector2& min, Vector2& max) const
{
	min = V2_ONE * 99999999.0f;
	max = V2_ONE * -99999999.0f;

	for (int i = 0; i < sectorWalls.size(); i++)
	{
		if (sectorWalls[i]->leftPoint.x < min.x) min.x = sectorWalls[i]->leftPoint.x;
		else if (sectorWalls[i]->leftPoint.x > max.x) max.x = sectorWalls[i]->leftPoint.x;

		if (sectorWalls[i]->rightPoint.x < min.x) min.x = sectorWalls[i]->rightPoint.x;
		else if (sectorWalls[i]->rightPoint.x > max.x) max.x = sectorWalls[i]->rightPoint.x;

		if (sectorWalls[i]->leftPoint.y < min.y) min.y = sectorWalls[i]->leftPoint.y;
		else if (sectorWalls[i]->leftPoint.y > max.y) max.y = sectorWalls[i]->leftPoint.y;

		if (sectorWalls[i]->rightPoint.y < min.y) min.y = sectorWalls[i]->rightPoint.y;
		else if (sectorWalls[i]->rightPoint.y > max.y) max.y = sectorWalls[i]->rightPoint.y;
	}
}

bool Sector::PointIsInsideSector(Vector2 point) const
{
	Vector2 min, max;
	GetMaxPoints(min, max);
	return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
}

Vector2 Sector::CalculateSectorCentroid() const
{
	Vector2 centroid = V2_ZERO;
	float sumArea = 0.0f, area = 0.0f;

	for (int i = 0; i < sectorWalls.size(); i++)
	{
		area = sectorWalls[i]->leftPoint.x * sectorWalls[i]->rightPoint.y - sectorWalls[i]->rightPoint.x * sectorWalls[i]->leftPoint.y;
		sumArea += area;
		centroid.x += (sectorWalls[i]->leftPoint.x + sectorWalls[i]->rightPoint.x) * area;
		centroid.y += (sectorWalls[i]->leftPoint.y + sectorWalls[i]->rightPoint.y) * area;
	}

	centroid /= (sumArea * 3.0f);
	return centroid;
}

bool Wall::VectorInFrontWall(Vector2 vector) const
{
	Vector2 wallVector = rightPoint - leftPoint;
	return vector.x * wallVector.y < wallVector.x * vector.y;
}

bool World::FindWallByID(unsigned long long id, int& wallIndx, int& sectorIndx) const
{
	for (int s = 0; s < numberOfSectors; s++)
	{
		for (int w = 0; w < sectorData[s].sectorWalls.size(); w++)
		{
			if (sectorData[s].sectorWalls[w]->wallID != id) continue;
			sectorIndx = s;
			wallIndx = w;
			return true;
		}
	}

	wallIndx = -1;
	sectorIndx = -1;
	return false;
}

bool World::FindWallByIDWithSector(unsigned long long id, int sectorIndx, int& wallIndx) const
{
	for (int w = 0; w < sectorData[sectorIndx].sectorWalls.size(); w++)
	{
		if (sectorData[sectorIndx].sectorWalls[w]->wallID != id) continue;
		wallIndx = w;
		return true;
	}

	wallIndx = -1;
	return false;
}
