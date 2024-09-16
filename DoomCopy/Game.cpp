#include "framework.h"
#include "Game.h"
#include "TransformComponent.h"

void Game::InitUpdate()
{
	long long nextGameTick = GetGameTickCount();
	loops = 0;
	startingPixel = 100;
	movePixel = startingPixel;

    entities.push_back(new GameObject());
}

void Game::MainUpdate()
{
    loops = 0;

    long long start = GetGameTickCount();
    while (GetGameTickCount() > nextGameTick && loops < MAX_FRAMESKIP)
    {
        // GameLoop

        size_t count = entities.size();
        for (size_t i = 0; i < count; i++) entities[i]->TickComponents();

        movePixel++;
        if ((movePixel - startingPixel) > 100) movePixel = startingPixel;
        //OLOG_LF("{0} | {1}", movePixel, loops);

        nextGameTick += SKIP_TICKS;
        loops++;
    }
}

long long Game::GetGameTickCount() const
{
	std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
}

Game::Game() : nextGameTick(0LL), loops(0), startingPixel(0), movePixel(0)
{
	startTime = std::chrono::high_resolution_clock::now();
    entities = std::vector<Entity*>();
}

Game::~Game()
{
    size_t size = entities.size();
    for (size_t i = 0; i < size; i++) delete entities[i];
    entities.clear();
}
