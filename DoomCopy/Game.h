#pragma once
#include "GameObjects.h"
#include "Component.h"
#include "VectorMath.h"
#include "CameraComponent.h"

#define DEFAULT_TICKS_PER_SECOND 30
#define SKIP_TICKS 1000 / DEFAULT_TICKS_PER_SECOND
#define MAX_FRAMESKIP 10

class Game
{
public:
	Game();
	~Game();

	void InitUpdate();
	void MainUpdate();

	void CaptureKeyPress(unsigned long long keycode);
	void CaptureKeyRelease(unsigned long long keycode);

	void CaptureMousePress(UINT btn);
	void CaptureMouseRelease(UINT btn);
	void CaptureMouseMovement(const POINTS* const mousePos, bool isClipped);

	size_t GetEntityCount() const { return entities.size(); }
	GameObject* GetGameObject(size_t indx) const { return dynamic_cast<GameObject*>(entities[indx]); }
	class Camera* GetMainCamera() const { return mainCamera; }
	class World* GetWorldRef() const { return world; }

private:
	std::vector<Entity*> entities;
	class Camera* mainCamera;
	class World* world;
	int loops;

	std::string mapToOpen = "asd.bsp";
};
