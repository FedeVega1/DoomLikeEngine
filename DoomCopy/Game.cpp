#include "pch.h"
#include "Game.h"
#include "CameraComponent.h"
#include "GameTime.h"
#include "Inputs.h"

Time Time::INS;
Input Input::INS;

void Game::InitUpdate()
{
    Time::INS.nextGameTick = Time::INS.GetGameTickCount();
	loops = 0;

    GameObject* cameraObject = new GameObject();
    mainCamera = cameraObject->AddComponent<Camera>();
    entities.push_back(cameraObject);
}

void Game::MainUpdate()
{
    loops = 0;

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    while (Time::INS.GetGameTickCount() > Time::INS.nextGameTick && loops < MAX_FRAMESKIP)
    {
        // GameLoop
        Input::INS.lastReleasedKey = KeyCode::None;

        size_t count = entities.size();
        for (size_t i = 0; i < count; i++) entities[i]->TickComponents();

        Time::INS.nextGameTick += SKIP_TICKS;
        loops++;
    }

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    Time::INS.deltaTime = std::chrono::duration<double, std::milli>(now - start).count();
}

Game::Game() : loops(0), mainCamera(nullptr)
{
    Time::INS = Time();
	Time::INS.startTime = std::chrono::high_resolution_clock::now();

    Input::INS = Input();

    entities = std::vector<Entity*>();
}

Game::~Game()
{
    size_t size = entities.size();
    for (size_t i = 0; i < size; i++) delete entities[i];
    entities.clear();
}

void Game::CaptureKeyPress(unsigned long long keycode) { Input::INS.GetKeyPress(static_cast<KeyCode>(keycode)); }

void Game::CaptureKeyRelease(unsigned long long keycode) { Input::INS.GetKeyRelease(static_cast<KeyCode>(keycode)); }
