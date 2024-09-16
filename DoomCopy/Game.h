#pragma once
#include "Component.h"
#include "GameObjects.h"

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

	size_t GetEntityCount() const { return entities.size(); }
	GameObject* GetGameObject(size_t indx) const { return dynamic_cast<GameObject*>(entities[indx]); }

	int GetPixel() const { return movePixel; }

private:
	std::vector<Entity*> entities;
	std::chrono::steady_clock::time_point startTime;
	long long nextGameTick;
	int loops, startingPixel, movePixel;

	long long GetGameTickCount() const;
};
