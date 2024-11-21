#include "pch.h"
#include "VectorMath.h"
#include "GameObjects.h"
#include "World.h"
#include <fstream>
#include <filesystem>
#include <bit>

World::World(Game* const gameRef, const std::string& mapFileName) : Entity(gameRef), numberOfSectors(0), sectorData(nullptr)
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
			OLOG_LF("Loading Sector: {0}", i);

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

				unsigned char portalFlags;
				mapFile.read((char*) & portalFlags, 1);

				wall.isPortal = (portalFlags & 0b1) == 0b1;
				wall.isConnection = (portalFlags & 0b10) == 0b10;

				mapFile.read((char*) intBuffer, intSize);
				wall.portalTargetSector = ByteArrayToInt(intBuffer, isLittleEndian);

				mapFile.read((char*) intBuffer, intSize);
				wall.portalTargetWall = ByteArrayToInt(intBuffer, isLittleEndian);

				sector.sectorWalls[j] = wall;
			}

			mapFile.read((char*) intBuffer, intSize);
			sector.topPoint = static_cast<float>(ByteArrayToInt(intBuffer, isLittleEndian));

			mapFile.read((char*) intBuffer, intSize);
			sector.bottomPoint = static_cast<float>(ByteArrayToInt(intBuffer, isLittleEndian));

			mapFile.read((char*) colorBuffer, colorSize);
			sector.floorColor = ByteArrayToColor(colorBuffer);

			mapFile.read((char*) colorBuffer, colorSize);
			sector.ceillingColor = ByteArrayToColor(colorBuffer);

			sector.sectorID = i;
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
	for (int i = 0; i < numberOfWalls; i++)
	{
		if (sectorWalls[i].isPortal)
			return true;
	}

	return false;
}

void Sector::GetPortalSectors(std::vector<int>*portalSectors, int ignoreSector) const
{
	for (int i = 0; i < numberOfWalls; i++)
	{
		if (!sectorWalls[i].isPortal || sectorWalls[i].portalTargetSector == ignoreSector) continue;
		portalSectors->push_back(sectorWalls[i].portalTargetSector);
	}
}

float Sector::GetAvrgDistanceToPoint(Vector2 point) const
{
	float avrgDistance = 0;
	for (int i = 0; i < numberOfWalls; i++)
		avrgDistance += Vector2::Distance(point, sectorWalls[i].GetAvrgMiddlePoint());

	return avrgDistance / numberOfWalls;
}

void Sector::GetMaxPoints(Vector2& min, Vector2& max) const
{
	min = V2_ONE * 99999999.0f;
	max = V2_ONE * -99999999.0f;

	for (int i = 0; i < numberOfWalls; i++)
	{
		if (sectorWalls[i].leftPoint.x < min.x) min.x = sectorWalls[i].leftPoint.x;
		else if (sectorWalls[i].leftPoint.x > max.x) max.x = sectorWalls[i].leftPoint.x;

		if (sectorWalls[i].rightPoint.x < min.x) min.x = sectorWalls[i].rightPoint.x;
		else if (sectorWalls[i].rightPoint.x > max.x) max.x = sectorWalls[i].rightPoint.x;

		if (sectorWalls[i].leftPoint.y < min.y) min.y = sectorWalls[i].leftPoint.y;
		else if (sectorWalls[i].leftPoint.y > max.y) max.y = sectorWalls[i].leftPoint.y;

		if (sectorWalls[i].rightPoint.y < min.y) min.y = sectorWalls[i].rightPoint.y;
		else if (sectorWalls[i].rightPoint.y > max.y) max.y = sectorWalls[i].rightPoint.y;
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

	for (int i = 0; i < numberOfWalls; i++)
	{
		area = sectorWalls[i].leftPoint.x * sectorWalls[i].rightPoint.y - sectorWalls[i].rightPoint.x * sectorWalls[i].leftPoint.y;
		sumArea += area;
		centroid.x += (sectorWalls[i].leftPoint.x + sectorWalls[i].rightPoint.x) * area;
		centroid.y += (sectorWalls[i].leftPoint.y + sectorWalls[i].rightPoint.y) * area;
	}

	centroid /= (sumArea * 3.0f);
	return centroid;
}
