#pragma once
class BaseComponent;

class Camera : public BaseComponent
{
public:
	virtual void Start() override;
	virtual void Tick() override;

protected:
	virtual void OnDestroy() override;

private:
	void DebugForward();
	void DebugBack();
	void DebugLeft();
	void DebugRight();
	void DebugRotRight();
	void DebugRotLeft();
};
