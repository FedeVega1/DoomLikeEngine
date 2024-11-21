#pragma once
class BaseComponent;
class Transform;

class Entity
{
	friend class Game;

public:
	template<class Comp>
	Comp* AddComponent()
	{
		Comp* component = new Comp();
		component->baseEntity = this;
		components.push_back(component);
		return component;
	}

	void RemoveComponent();
	void GetComponent();
	Game* GetGameRef() const { return mainGame; }

	Entity(Game* const gameRef);
	~Entity();

protected:
	Game* mainGame;

	virtual void Hack() = 0;

private:
	std::vector<BaseComponent*> components;

	void PreTickComponents();
	void TickComponents();
	void AfterTickComponents();
};

class GameObject : public Entity
{
public:
	GameObject(Game* const gameRef);
	Transform* GetTransform() const { return transform; }

protected:
	virtual void Hack() override { }

private:
	Transform* transform;
};