#include "pch.h"
#include "VectorMath.h"
#include <fstream>
#include <bit>
#include <filesystem>
#include "WorldDataTypes.h"
#include "FileManager.h"

FileManager::FileManager() : intBuffer(0), pointBuffer(0), colorBuffer(0), idBuffer(0), pointFBuffer(0)
{

}

FileManager::~FileManager()
{

}

void FileManager::GetMapData(const std::string& mapFileName, MapData& outMapData)
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
		outMapData.numberOfSectors = ByteArrayToInt(intBuffer, isLittleEndian);
		OLOG_LF("Number of Sectors: {0}", outMapData.numberOfSectors);

		outMapData.sectorData = new Sector[outMapData.numberOfSectors];
		for (int i = 0; i < outMapData.numberOfSectors; i++)
		{
			Sector& sector = outMapData.sectorData[i] = Sector();

			mapFile.read((char*) intBuffer, intSize);
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

		OLOG_L("Loaded all Sectors Correct!");
		mapFile.read((char*) intBuffer, intSize);

		mapFile.read((char*) intBuffer, intSize);
		outMapData.numberOfSubSectors = ByteArrayToInt(intBuffer, isLittleEndian);

		OLOG_LF("Reading SubSector Data, number of sub sectors: {0}", outMapData.numberOfSubSectors);
		ReadSubSectorData(&mapFile, isLittleEndian, outMapData);

		mapFile.read((char*) intBuffer, intSize);
		outMapData.numberOfBSPNodes = ByteArrayToInt(intBuffer, isLittleEndian);

		OLOG_LF("Reading BSP Data, number of nodes: {0}", outMapData.numberOfBSPNodes);

		ReadBSPNode(&mapFile, isLittleEndian, outMapData);

		//OLOG_LF("Loaded {0} BSP nodes from a total of {1}", nodeCount, maxNumberOfBSPNodes);
		//OLOG_LF("Loaded {0} front Nodes and {1} back nodes", front, back);

		OLOG_LF("The map {0} loaded successfully!", mapFileName);
		mapFile.close();
	}
	catch (std::system_error& e)
	{
		char buffer[50];
		strerror_s(buffer, 50, errno);
		OLOG_CF("Couldn't load file {0}!! - Error: {1}, {2}", mapFileName, e.code().message(), buffer);
	}
}

void FileManager::ReadBSPNode(std::ifstream* stream, const bool& isLittleEndian, MapData& mapData)
{
	unsigned char singleByte;
	BSPNode** nodeArray = new BSPNode*[mapData.numberOfBSPNodes];

	for (int i = 0; i < mapData.numberOfBSPNodes; i++)
	{
		BSPNode& node = *(nodeArray[i] = new BSPNode());

		stream->read((char*) &singleByte, 1);
		node.childFlag = singleByte;

		stream->read((char*) intBuffer, intSize);
		node.nodeID = ByteArrayToUInt(intBuffer, isLittleEndian);

		Splitter splitter;

		stream->read((char*) pointBuffer, pointSize);
		splitter.startPoint = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

		stream->read((char*) pointBuffer, pointSize);
		splitter.segment = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

		stream->read((char*) pointFBuffer, pointFSize);
		splitter.dir = ByteArrayToVector2(pointFBuffer, isLittleEndian);

		node.splitter = splitter;

		stream->read((char*) intBuffer, intSize);
		node.parentID = ByteArrayToUInt(intBuffer, isLittleEndian);

		stream->read((char*) intBuffer, intSize);
		node.subSectorID = ByteArrayToUInt(intBuffer, isLittleEndian);

		BoundingBox bBox;
		stream->read((char*) pointBuffer, pointSize);
		bBox.topPoint = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

		stream->read((char*) pointBuffer, pointSize);
		bBox.bottomPoint = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

		OLOG_LF("Loaded Node {0} with parentID {1} and childFlag {2}", node.nodeID, node.parentID, node.childFlag);
	}

	SortData data = SortData<BSPNode*>
	{
		nodeArray,
		[](BSPNode* node) -> int { return node->nodeID; },
		false
	};

	Quick_Sort(data, 0, mapData.numberOfBSPNodes - 1);

	for (int i = 0; i < mapData.numberOfBSPNodes; i++)
	{
		if (nodeArray[i]->childFlag == 0xFF)
		{
			mapData.rootNode = nodeArray[i];
			continue;
		}

		if (nodeArray[i]->parentID != 0xFFFFFFFF)
			nodeArray[i]->parentNode = nodeArray[nodeArray[i]->parentID];

		if (nodeArray[i]->parentID >= mapData.numberOfBSPNodes)
		{
			OLOG_EF("Node {0} has an invalid parentID: {1}", i, nodeArray[i]->parentID);
			return;
		}

		if (nodeArray[i]->subSectorID != 0xFFFFFFFF)
		{
			nodeArray[i]->subSector = GetSubSector(nodeArray[i]->subSectorID, mapData);
			if (nodeArray[i]->subSector) OLOG_LF("Registered subsector {0} into node {1}", nodeArray[i]->subSectorID, nodeArray[i]->nodeID);
		}

		if (nodeArray[i]->childFlag == 0xBB)
		{
			nodeArray[nodeArray[i]->parentID]->frontNode = nodeArray[i];
			continue;
		}

		if (nodeArray[i]->childFlag != 0xCC) continue;
		nodeArray[nodeArray[i]->parentID]->backNode = nodeArray[i];
	}

	delete[] nodeArray;
	OLOG_L("Loaded all BSP nodes correct!");
}

SubSector* FileManager::GetSubSector(const unsigned int& subSectorID, const MapData& mapData) const 
{
	for (int i = 0; i < mapData.numberOfSubSectors; i++)
	{
		if (mapData.subSectorData[i].subSectorID != subSectorID) continue;
		return &mapData.subSectorData[i];
	}

	return nullptr;
}

void FileManager::GetWallFromFile(std::ifstream* stream, const bool& isLittleEndian, Wall& wall, const MapData& mapData) const
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
	wall.portalTargetSectorID = ByteArrayToInt(intBuffer, isLittleEndian);

	stream->read((char*) intBuffer, intSize);
	wall.portalTargetWall = ByteArrayToInt(intBuffer, isLittleEndian);

	stream->read((char*) idBuffer, idSize);
	wall.wallID = ByteArrayToULL(idBuffer, isLittleEndian);

	stream->read((char*) intBuffer, intSize);
	unsigned int sectorID = ByteArrayToUInt(intBuffer, isLittleEndian);

	for (int i = 0; i < mapData.numberOfSectors; i++)
	{
		if (sectorID != mapData.sectorData[i].sectorID) continue;
		wall.parentSector = &mapData.sectorData[i];
		mapData.sectorData[i].sectorWalls.push_back(wall);
		mapData.sectorData[i].sectorCenter = mapData.sectorData[i].CalculateSectorCentroid();
		return;
	}

	OLOG_CF("Couldn't find sector ID {0}", sectorID);
	throw std::system_error(1, std::generic_category());
}

void FileManager::ReadSubSectorData(std::ifstream* stream, const bool& isLittleEndian, MapData& mapData)
{
	mapData.subSectorData = new SubSector[mapData.numberOfSubSectors];

	for (int i = 0; i < mapData.numberOfSubSectors; i++)
	{ 
		SubSector newSector;

		stream->read((char*) intBuffer, intSize);
		newSector.subSectorID = ByteArrayToUInt(intBuffer, isLittleEndian);

		stream->read((char*) intBuffer, intSize);
		int numberOfWalls = ByteArrayToInt(intBuffer, isLittleEndian);

		for (int w = 0; w < numberOfWalls; w++)
		{
			Wall newWall;
			GetWallFromFile(stream, isLittleEndian, newWall, mapData);
			newSector.subSectorWalls.push_back(newWall);
		}

		mapData.subSectorData[i] = newSector;
		OLOG_LF("Loaded SubSector with ID {0}", newSector.subSectorID);
	}

	OLOG_L("Loaded all SubSectors correct!");
}

int FileManager::ByteArrayToInt(const unsigned char* const byteArray, const bool& isLittleEndian) const
{
	if (isLittleEndian) return (static_cast<int>(byteArray[3]) << 24) | (static_cast<int>(byteArray[2]) << 16) | (static_cast<int>(byteArray[1]) << 8) | static_cast<int>(byteArray[0]);
	return (static_cast<int>(byteArray[0]) << 24) | (static_cast<int>(byteArray[1]) << 16) | (static_cast<int>(byteArray[2]) << 8) | static_cast<int>(byteArray[3]);
}

unsigned int FileManager::ByteArrayToUInt(const unsigned char* const byteArray, const bool& isLittleEndian) const
{
	if (isLittleEndian) return (static_cast<unsigned int>(byteArray[3]) << 24) | (static_cast<unsigned int>(byteArray[2]) << 16) | (static_cast<unsigned int>(byteArray[1]) << 8) | static_cast<unsigned int>(byteArray[0]);
	return (static_cast<unsigned int>(byteArray[0]) << 24) | (static_cast<unsigned int>(byteArray[1]) << 16) | (static_cast<unsigned int>(byteArray[2]) << 8) | static_cast<unsigned int>(byteArray[3]);
}

unsigned long long FileManager::ByteArrayToULL(const unsigned char* byteArray, const bool& isLittleEndian) const
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

Vector2Int FileManager::ByteArrayToVector2Int(const unsigned char* const byteArray, const bool& isLittleEndian) const
{
	const int intSize = sizeof(int);
	unsigned char x[intSize]{ byteArray[0], byteArray[1], byteArray[2], byteArray[3] };
	unsigned char y[intSize]{ byteArray[4], byteArray[5], byteArray[6], byteArray[7] };
	return Vector2Int(ByteArrayToInt(x, isLittleEndian), ByteArrayToInt(y, isLittleEndian));
}

float FileManager::ByteArrayToFloat(const unsigned char* const byteArray, const bool& isLittleEndian) const
{
	return static_cast<float>(ByteArrayToInt(byteArray, isLittleEndian));
}

Vector2 FileManager::ByteArrayToVector2(const unsigned char* const byteArray, const bool& isLittleEndian) const
{
	const int floatSize = sizeof(float);
	unsigned char x[floatSize]{ byteArray[0], byteArray[1], byteArray[2], byteArray[3] };
	unsigned char y[floatSize]{ byteArray[4], byteArray[5], byteArray[6], byteArray[7] };
	return Vector2Int(ByteArrayToInt(x, isLittleEndian), ByteArrayToInt(y, isLittleEndian));
}

Color FileManager::ByteArrayToColor(const unsigned char* const byteArray) const { return Color(byteArray[0], byteArray[1], byteArray[2]); }
