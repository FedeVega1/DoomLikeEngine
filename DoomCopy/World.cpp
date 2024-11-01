#include "pch.h"
#include "VectorMath.h"
#include "GameObjects.h"
#include "World.h"
#include <fstream>
#include <filesystem>

World::World(const std::string& mapFileName) : Entity()
{
	const int intSize = sizeof(int), pointSize = intSize * 2, colorSize = sizeof(char) * 3;

	char intBuffer[intSize], pointBuffer[pointSize], colorBuffer[colorSize];

	OLOG_LF("Working Directory: {0}", std::filesystem::current_path().string());

	std::ifstream mapFile;
	mapFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try 
	{
		mapFile.open(mapFileName, std::ios_base::in | std::ios_base::binary);
		OLOG_LF("Reading Map: {0}", mapFileName);

		mapFile.read(intBuffer, intSize);
		numberOfSectors = ByteArrayToInt(intBuffer);
		OLOG_LF("Number of Sectors: {0}", numberOfSectors);

		sectorData = new Sector[numberOfSectors];
		for (int i = 0; i < numberOfSectors; i++)
		{
			Sector sector = Sector();
			mapFile.read(intBuffer, intSize);

			sector.numberOfWalls = ByteArrayToInt(intBuffer);

			sector.sectorWalls = new Wall[sector.numberOfWalls];
			for (int j = 0; j < sector.numberOfWalls; j++)
			{
				Wall wall = Wall();

				mapFile.read(pointBuffer, pointSize);
				wall.leftPoint = ByteArrayToVector2Int(pointBuffer);

				mapFile.read(pointBuffer, pointSize);
				wall.rightPoint = ByteArrayToVector2Int(pointBuffer);

				mapFile.read(colorBuffer, colorSize);
				wall.color = ByteArrayToColor(colorBuffer);

				sector.sectorWalls[j] = wall;
			}

			mapFile.read(intBuffer, intSize);
			sector.topPoint = ByteArrayToInt(intBuffer);

			mapFile.read(intBuffer, intSize);
			sector.bottomPoint = ByteArrayToInt(intBuffer);

			mapFile.read(colorBuffer, colorSize);
			sector.floorColor = ByteArrayToColor(colorBuffer);

			mapFile.read(colorBuffer, colorSize);
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

int World::ByteArrayToInt(const char* byteArray)
{
	return (static_cast<int>(byteArray[3]) << 24) | (static_cast<int>(byteArray[2]) << 16) | (static_cast<int>(byteArray[1]) << 8) | static_cast<int>(byteArray[0]);
}

Vector2Int World::ByteArrayToVector2Int(const char* byteArray)
{
	const int intSize = sizeof(int);
	char x[intSize] { byteArray[0], byteArray[1], byteArray[2], byteArray[3] };
	char y[intSize] { byteArray[4], byteArray[5], byteArray[6], byteArray[7] };
	return Vector2Int(ByteArrayToInt(x), ByteArrayToInt(y));
}

Color World::ByteArrayToColor(const char* byteArray) { return Color(byteArray[0], byteArray[1], byteArray[2]); }
