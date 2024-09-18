#include "pch.h"
#include "GameObjects.h"
#include "Component.h"
#include "TransformComponent.h"

void Entity::RemoveComponent()
{

}

void Entity::GetComponent()
{

}

void Entity::TickComponents()
{
	size_t size = components.size();
	for (size_t i = 0; i < size; i++)
	{
		if (!components[i]->firstTick) components[i]->Start();
		components[i]->Tick();
	}
}

Entity::Entity()
{
	components = std::vector<BaseComponent*>();
}

Entity::~Entity()
{
	size_t size = components.size();
	for (size_t i = 0; i < size; i++)
	{
		components[i]->OnDestroy();
		delete components[i];
	}

	components.clear();
}

GameObject::GameObject() : Entity()
{
	transform = AddComponent<Transform>();
}
