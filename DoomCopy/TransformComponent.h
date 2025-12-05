#pragma once
#include "VectorMath.h"

class Transform : public BaseComponent
{
public:
	friend class Game;
	friend class World;

	virtual void Start() override;
	virtual void Tick() override;

	void SetPos(Vector3 newPos);

	void SetRotation(float newAngle) { rot = newAngle; }
	void Rotate(float angle);

	void TeleportTo(Vector3 newPos);

	Vector3 GetPos() const { return pos; }
	float GetRot() const { return rot; }

	Vector3 GetForwardVector() const 
	{
		int currentRot = (int) std::roundf(rot);
		return Vector3((float) SCTABLE.sin[currentRot], (float) SCTABLE.cos[currentRot], 0);
	}

	Vector3 GetLeftVector() const 
	{ 
		int currentRot = (int) std::roundf(rot);
		return Vector3((float) -SCTABLE.cos[currentRot], (float) SCTABLE.sin[currentRot], 0);
	}

	Vector3 GetUpVector() const { return Vector3::Cross(GetForwardVector(), -GetLeftVector()); }

	int GetCurrentSector() const { return currentSector; }

	Transform();

protected:
	virtual void OnDestroy() override;

private:
	int currentSector;
	float rot;
	Vector3 pos;
	Vector3 lastPos;

	void SetCurrentSector(int sectorNumber) { currentSector = sectorNumber; }
};
