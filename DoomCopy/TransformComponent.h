#pragma once
#include "VectorMath.h"

class Transform : public BaseComponent
{
public:
	virtual void Start() override;
	virtual void Tick() override;

	Vector3 GetPos() const { return pos; }
	int GetRot() const { return rot; }

	Transform();

protected:
	virtual void OnDestroy() override;

private:
	Vector3 pos;
	int rot;
};
