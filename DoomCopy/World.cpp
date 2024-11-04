#include "pch.h"
#include "VectorMath.h"
#include "GameObjects.h"
#include "World.h"
#include <fstream>
#include <filesystem>
#include <bit>

World::World(const std::string& mapFileName) : Entity()
{
	const int intSize = sizeof(int), pointSize = intSize * 2, colorSize = sizeof(char) * 3;

	unsigned char intBuffer[intSize], pointBuffer[pointSize], colorBuffer[colorSize];

	OLOG_LF("Working Directory: {0}", std::filesystem::current_path().string());

	std::ifstream mapFile;
	mapFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try 
	{
		mapFile.open(mapFileName, std::ios_base::in | std::ios_base::binary);
		OLOG_LF("Reading Map: {0}", mapFileName);

		mapFile.read((char*) intBuffer, intSize);
		bool isLittleEndian = intBuffer[0] == 0x1;

		mapFile.read((char*) intBuffer, intSize);
		numberOfSectors = ByteArrayToInt(intBuffer, isLittleEndian);
		OLOG_LF("Number of Sectors: {0}", numberOfSectors);

		sectorData = new Sector[numberOfSectors];
		for (int i = 0; i < numberOfSectors; i++)
		{
			Sector sector = Sector();
			mapFile.read((char*) intBuffer, intSize);

			sector.numberOfWalls = ByteArrayToInt(intBuffer, isLittleEndian);

			sector.sectorWalls = new Wall[sector.numberOfWalls];
			for (int j = 0; j < sector.numberOfWalls; j++)
			{
				Wall wall = Wall();

				mapFile.read((char*) pointBuffer, pointSize);
				wall.leftPoint = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

				mapFile.read((char*) pointBuffer, pointSize);
				wall.rightPoint = ByteArrayToVector2Int(pointBuffer, isLittleEndian);

				OLOG_LF("Sector: {0} - Wall: {1} - LeftPoint {2} | RightPoint {3}", i, j, wall.leftPoint.ToString(), wall.rightPoint.ToString());

				mapFile.read((char*) colorBuffer, colorSize);
				wall.color = ByteArrayToColor(colorBuffer);

				sector.sectorWalls[j] = wall;
			}

			mapFile.read((char*) intBuffer, intSize);
			sector.topPoint = ByteArrayToInt(intBuffer, isLittleEndian);

			mapFile.read((char*) intBuffer, intSize);
			sector.bottomPoint = ByteArrayToInt(intBuffer, isLittleEndian);

			mapFile.read((char*) colorBuffer, colorSize);
			sector.floorColor = ByteArrayToColor(colorBuffer);

			mapFile.read((char*) colorBuffer, colorSize);
			sector.ceillingColor = ByteArrayToColor(colorBuffer);

			sectorData[i] = sector;
			OLOG_LF("Loaded Sector: {0} Correct", i);
		}

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

World::~World()
{
	Entity::~Entity();

	for (int i = 0; i < numberOfSectors; i++)
	{
		delete[] sectorData[i].sectorWalls;
		sectorData[i].sectorWalls = nullptr;
	}

	delete[] sectorData;
	sectorData = nullptr;
}

int World::ByteArrayToInt(const unsigned char* const byteArray, bool isLittleEndian) const
{
	if (isLittleEndian) return (static_cast<int>(byteArray[3]) << 24) | (static_cast<int>(byteArray[2]) << 16) | (static_cast<int>(byteArray[1]) << 8) | static_cast<int>(byteArray[0]);
	return (static_cast<int>(byteArray[0]) << 24) | (static_cast<int>(byteArray[1]) << 16) | (static_cast<int>(byteArray[2]) << 8) | static_cast<int>(byteArray[3]);
}

Vector2Int World::ByteArrayToVector2Int(const unsigned char* const byteArray, bool isLittleEndian) const
{
	const int intSize = sizeof(int);
	unsigned char x[intSize] { byteArray[0], byteArray[1], byteArray[2], byteArray[3] };
	unsigned char y[intSize] { byteArray[4], byteArray[5], byteArray[6], byteArray[7] };
	return Vector2Int(ByteArrayToInt(x, isLittleEndian), ByteArrayToInt(y, isLittleEndian));
}

Color World::ByteArrayToColor(const unsigned char* const byteArray) const { return Color(byteArray[0], byteArray[1], byteArray[2]); }
