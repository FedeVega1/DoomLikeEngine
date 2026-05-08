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

void Entity::PreTickComponents()
{
	size_t size = components.size();
	for (size_t i = 0; i < size; i++)
	{
		if (!components[i]->firstTick) components[i]->Start();
		components[i]->PreTick();
	}
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

void Entity::AfterTickComponents()
{
	size_t size = components.size();
	for (size_t i = 0; i < size; i++)
	{
		if (!components[i]->firstTick) components[i]->Start();
		components[i]->AfterTick();
	}
}

Entity::Entity(Game* const gameRef)
{
	components = std::vector<BaseComponent*>();
	mainGame = gameRef;
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

GameObject::GameObject(Game* const gameRef) : Entity(gameRef)
{
	transform = AddComponent<Transform>();
}
