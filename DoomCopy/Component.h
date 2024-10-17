#pragma once

class BaseComponent
{
	friend class Entity;

public:
	virtual void Start() { firstTick = true; };
	virtual void Tick() = 0;

	BaseComponent() : baseEntity(nullptr), firstTick(false), baseGameObject(nullptr)
	{ }

	class GameObject* GetGameObject() 
	{  
		if (!baseGameObject) baseGameObject = dynamic_cast<GameObject*>(baseEntity);
		return baseGameObject;
	}

protected:
	class Entity* baseEntity;
	virtual void OnDestroy() = 0;

	Transform* GetTransform() { return GetGameObject()->GetTransform(); }

private:
	bool firstTick;
	class GameObject* baseGameObject;
};
