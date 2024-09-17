#include "framework.h"
#include "Game.h"
#include "TransformComponent.h"
#include "GameTime.h"

Time Time::INS;

void Game::InitUpdate()
{
    Time::INS.nextGameTick = Time::INS.GetGameTickCount();
	loops = 0;
    entities.push_back(new GameObject());
}

void Game::MainUpdate()
{
    loops = 0;

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    while (Time::INS.GetGameTickCount() > Time::INS.nextGameTick && loops < MAX_FRAMESKIP)
    {
        // GameLoop

        size_t count = entities.size();
        for (size_t i = 0; i < count; i++) entities[i]->TickComponents();

        Time::INS.nextGameTick += SKIP_TICKS;
        loops++;
    }

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    Time::INS.deltaTime = std::chrono::duration<double, std::milli>(now - start).count();
}

Game::Game() : loops(0)
{
    Time::INS = Time();
	Time::INS.startTime = std::chrono::high_resolution_clock::now();
    entities = std::vector<Entity*>();
}

Game::~Game()
{
    size_t size = entities.size();
    for (size_t i = 0; i < size; i++) delete entities[i];
    entities.clear();
}
