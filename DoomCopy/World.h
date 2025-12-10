#pragma once
#include "WorldDataTypes.h"

class World : public Entity
{
public:
	friend class Game;
	friend class Camera;

	bool CheckIfPositionInsideSector(const Vector3& pos, int sector) const;
	bool CheckIfPositionInsideSector(const Vector3& pos, int* const sector) const;

	bool FindWallByID(const unsigned long long& id, int& wallIndx, int& subSectorIndx);
	bool FindWallByIDSector(const unsigned long long& id, int& wallIndx, int& sectorIndx) const;
	bool FindWallByIDWithSector(const unsigned long long& id, int sectorIndx, int& wallIndx) const;

	Sector GetSectorData(int sectorIndx) const { return sectorData[sectorIndx]; }

	World(Game* const gameRef, const std::string& mapFileName);
	~World();

protected:
	virtual void Hack() override {}

private:
	Sector* sectorData;
	SubSector* subSectorData;
	int numberOfSectors;
	class BSPNode* rootNode;
	int maxNumberOfBSPNodes;
	int maxNumberOfSubSectors;
	int maxNumberOfWalls;

	void CountTotalNumberOfWalls();
	void FindWallsPortal();
};
