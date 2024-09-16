#pragma once

class Transform : public BaseComponent
{
public:
	virtual void Start() override;
	virtual void Tick() override;

	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetZ() const { return z; }

	int GetRot() const { return rot; }

protected:
	virtual void OnDestroy() override;

private:
	int x, y, z;
	int rot;
};
