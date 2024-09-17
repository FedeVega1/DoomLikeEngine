#pragma once

class BaseComponent
{
	friend class Entity;

public:
	virtual void Start() { firstTick = true; };
	virtual void Tick() = 0;

	BaseComponent() : firstTick(false)
	{}

protected:
	virtual void OnDestroy() = 0;

private:
	bool firstTick;
};
