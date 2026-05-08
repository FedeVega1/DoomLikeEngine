#pragma once

class World : public Entity
{
public:
	friend class Game;
	friend class Camera;

	bool CheckIfPositionInsideSector(const Vector3& pos, int sector) const;
	bool CheckIfPositionInsideSector(const Vector3& pos, int* const sector) const;

	const Sector* GetSectorByID(uint32_t sectorID) const { return bspMap.FindSectorByID(sectorID); }
	const Sector& GetSector(int sectorIndx) const { return bspMap.sectors[sectorIndx]; }

	World(Game* const gameRef, const std::string& mapFileName);
	~World() = default;

protected:
	virtual void Hack() override {}

private:
	BSPMap bspMap;

	void ComputeSectorCenters();
	bool SectorContainsPoint(uint32_t sectorID, const Vector2& point) const;
};
