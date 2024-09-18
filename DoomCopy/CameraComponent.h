#pragma once
class BaseComponent;

class Camera : public BaseComponent
{
	friend class Game;

public:
	virtual void Start() override;
	virtual void Tick() override;

	Camera() : BaseComponent(), world(nullptr)
	{ }

protected:
	virtual void OnDestroy() override;

private:
	class World* world;

	void ClipBehindCamera(Vector3& pointA, const Vector3& pointB);

	void DebugForward();
	void DebugBack();
	void DebugLeft();
	void DebugRight();
	void DebugRotRight();
	void DebugRotLeft();
};
