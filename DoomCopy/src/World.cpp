#include "pch.h"
#include "GameObjects.h"
#include "World.h"

World::World(Game* const gameRef, const std::string& mapFileName) : Entity(gameRef)
{
	BSPFileReader reader;
	if (!reader.Read(mapFileName, bspMap))
		OLOG_CF("Failed to load map: {0}", mapFileName);

	ComputeSectorCenters();
}

void World::ComputeSectorCenters()
{
	for (auto& sector : bspMap.sectors)
	{
		Vector2 centroid;
		float sumArea = 0.0f;

		for (const auto& subSector : bspMap.subSectors)
		{
			for (const auto& wall : subSector.walls)
			{
				if (wall.parentSectorID != sector.sectorID) continue;

				float area = wall.leftPoint.x * wall.rightPoint.y - wall.rightPoint.x * wall.leftPoint.y;
				sumArea += area;
				centroid.x += (wall.leftPoint.x + wall.rightPoint.x) * area;
				centroid.y += (wall.leftPoint.y + wall.rightPoint.y) * area;
			}
		}

		if (std::abs(sumArea) > K_EPSILON)
			sector.center = centroid / (sumArea * 3.0f);
	}
}

bool World::SectorContainsPoint(uint32_t sectorID, const Vector2& point) const
{
	Vector2 min(99999999.0f, 99999999.0f);
	Vector2 max(-99999999.0f, -99999999.0f);
	bool found = false;

	for (const auto& subSector : bspMap.subSectors)
	{
		for (const auto& wall : subSector.walls)
		{
			if (wall.parentSectorID != sectorID) continue;
			found = true;

			if (wall.leftPoint.x < min.x) min.x = wall.leftPoint.x;
			if (wall.leftPoint.x > max.x) max.x = wall.leftPoint.x;
			if (wall.rightPoint.x < min.x) min.x = wall.rightPoint.x;
			if (wall.rightPoint.x > max.x) max.x = wall.rightPoint.x;

			if (wall.leftPoint.y < min.y) min.y = wall.leftPoint.y;
			if (wall.leftPoint.y > max.y) max.y = wall.leftPoint.y;
			if (wall.rightPoint.y < min.y) min.y = wall.rightPoint.y;
			if (wall.rightPoint.y > max.y) max.y = wall.rightPoint.y;
		}
	}

	if (!found) return false;
	return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
}

bool World::CheckIfPositionInsideSector(const Vector3& pos, int sector) const
{
	if (sector < 0 || sector >= static_cast<int>(bspMap.sectors.size())) return false;
	return SectorContainsPoint(bspMap.sectors[sector].sectorID, pos.XY());
}

bool World::CheckIfPositionInsideSector(const Vector3& pos, int* const sector) const
{
	*sector = -1;

	for (int i = 0; i < static_cast<int>(bspMap.sectors.size()); i++)
	{
		if (!SectorContainsPoint(bspMap.sectors[i].sectorID, pos.XY())) continue;
		*sector = i;
		return true;
	}

	return false;
}
