#include "pch.h"
#include "Game.h"
#include "CameraComponent.h"
#include "GameTime.h"
#include "Inputs.h"
#include "TransformComponent.h"
#include "World.h"

Time Time::INS;
Input Input::INS;

void Game::InitUpdate()
{
    Time::INS.nextGameTick = Time::INS.GetGameTickCount();
	loops = 0;

    world = new World();
    entities.push_back(world);

    GameObject* cameraObject = new GameObject();
    mainCamera = cameraObject->AddComponent<Camera>();
    mainCamera->world = world;
    entities.push_back(cameraObject);

    cameraObject->GetTransform()->SetPos(Vector3(70, -110, 0));
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
    Time::INS.deltaTime = std::chrono::duration<double, std::micro>(now - start).count();
}

Game::Game() : loops(0), mainCamera(nullptr), world(nullptr)
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
