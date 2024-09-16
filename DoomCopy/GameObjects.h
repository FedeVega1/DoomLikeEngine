#pragma once

class Transform;

class Entity
{
	friend class Game;

public:
	template<class Comp>
	Comp* AddComponent();

	void RemoveComponent();
	void GetComponent();

	Entity();
	~Entity();

protected:
	virtual void Hack() = 0;

private:
	std::vector<BaseComponent*> components;

	void TickComponents();
};

class GameObject : public Entity
{
public:
	GameObject();
	Transform* GetTransform() const { return transform; }

protected:
	virtual void Hack() override { }

private:
	Transform* transform;
};