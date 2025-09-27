#include "pch.h"
#include "VectorMath.h"
#include "GameObjects.h"
#include "World.h"
#include "FileManager.h"

World::World(Game* const gameRef, const std::string& mapFileName) : Entity(gameRef)
{
	MapData data;
	FManager.GetMapData(mapFileName, data);

	numberOfSectors = data.numberOfSectors;
	sectorData = data.sectorData;
	subSectorData = data.subSectorData;
	maxNumberOfBSPNodes = data.numberOfBSPNodes;
	rootNode = data.rootNode;
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

	delete rootNode;
}

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
		if (!sectorWalls[i]->isPortal || sectorWalls[i]->portalTargetSectorID == ignoreSector) continue;
		portalSectors->push_back(sectorWalls[i]->portalTargetSectorID);
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

bool Splitter::VectorInFront(Vector2 vector) const { return vector.x * segment.y < segment.x * vector.y; }

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
