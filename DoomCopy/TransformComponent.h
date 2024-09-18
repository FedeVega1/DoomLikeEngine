#pragma once
#include "VectorMath.h"

class Transform : public BaseComponent
{
public:
	virtual void Start() override;
	virtual void Tick() override;

	void SetPos(Vector3 newPos);

	void SetRotation(float newAngle) { rot = newAngle; }
	void Rotate(float angle);

	void TeleportTo(Vector3 newPos);

	Vector3 GetPos() const { return pos; }
	float GetRot() const { return rot; }

	Vector3 GetForwardVector() const { return Vector3((float) SCTABLE.sin[(int) std::roundf(rot)], 0, (float) SCTABLE.cos[(int)std::roundf(rot)]); }
	Vector3 GetRightVector() const { return Vector3((float) SCTABLE.cos[(int)std::roundf(rot)], 0 , (float) SCTABLE.sin[(int)std::roundf(rot)]); }

	Transform();

protected:
	virtual void OnDestroy() override;

private:
	Vector3 pos;
	float rot;
};
