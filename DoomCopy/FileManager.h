#pragma once

struct MapData
{
	int numberOfSectors, numberOfBSPNodes, numberOfSubSectors;
	struct Sector* sectorData;
	struct SubSector* subSectorData;
	class BSPNode* rootNode;
};

class FileManager
{
public:
	FileManager();
	~FileManager();

	void GetMapData(const std::string& mapName, MapData& outMapData);

private:
	static const int intSize = sizeof(int), pointSize = intSize * 2, colorSize = sizeof(char) * 3, idSize = sizeof(unsigned long long), pointFSize = sizeof(float) * 2;
	unsigned char intBuffer[intSize], pointBuffer[pointSize], colorBuffer[colorSize], idBuffer[idSize], pointFBuffer[pointFSize];

	const char BSPVersionSize = 2;
	char const BSPVersion[2]{ 00, 07 };

	int ByteArrayToInt(const unsigned char* const byteArray, const bool& isLittleEndian) const;
	unsigned int ByteArrayToUInt(const unsigned char* const byteArray, const bool& isLittleEndian) const;
	Vector2Int ByteArrayToVector2Int(const unsigned char* const byteArray, const bool& isLittleEndian) const;
	Color ByteArrayToColor(const unsigned char* const byteArray) const;
	unsigned long long ByteArrayToULL(const unsigned char* byteArray, const bool& isLittleEndian) const;
	Vector2 ByteArrayToVector2(const unsigned char* const byteArray, const bool& isLittleEndian) const;
	float ByteArrayToFloat(const unsigned char* const byteArray, const bool& isLittleEndian) const;
	void ReadBSPNode(std::ifstream* stream, const bool& isLittleEndian, MapData& mapData);
	void GetWallFromFile(std::ifstream* stream, const bool& isLittleEndian, Wall& wall, const MapData& mapData) const;
	void ReadSubSectorData(std::ifstream* stream, const bool& isLittleEndian, MapData& mapData);
	SubSector* GetSubSector(const unsigned int& subSectorID, const MapData& mapData) const;
};

static FileManager FManager;
