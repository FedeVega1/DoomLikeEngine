#include "pch.h"
#include "Game.h"
#include "CameraComponent.h"
#include "GameTime.h"
#include "TransformComponent.h"
#include "Inputs.h"
#include "World.h"

Time Time::INS = Time();
Input Input::INS = Input();

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
        Input::INS.ProcessInputs();

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
	Time::INS.startTime = std::chrono::high_resolution_clock::now();
    entities = std::vector<Entity*>();
}

Game::~Game()
{
    size_t size = entities.size();
    for (size_t i = 0; i < size; i++) delete entities[i];
    entities.clear();
}

void Game::CaptureKeyPress(unsigned long long keycode)
{ 
    KeyCode key = static_cast<KeyCode>(keycode);
    if (key == KeyCode::Esc) ReleaseCapture();
    Input::INS.GetKeyPress(key);
}

void Game::CaptureKeyRelease(unsigned long long keycode) { Input::INS.GetKeyRelease(static_cast<KeyCode>(keycode)); }

void Game::CaptureMousePress(UINT btn) 
{ 
    switch (btn)
    {
        case 0: Input::INS.GetKeyPress(KeyCode::LeftMouse); break;
        case 1: Input::INS.GetKeyPress(KeyCode::RightMouse); break;
        case 2: Input::INS.GetKeyPress(KeyCode::MiddleMouse); break;
        case 3: Input::INS.GetKeyPress(KeyCode::X1Mouse); break;
        case 4: Input::INS.GetKeyPress(KeyCode::X2Mouse); break;
    }
}

void Game::CaptureMouseRelease(UINT btn) 
{
    switch (btn)
    {
        case 0: Input::INS.GetKeyRelease(KeyCode::LeftMouse); break;
        case 1: Input::INS.GetKeyRelease(KeyCode::RightMouse); break;
        case 2: Input::INS.GetKeyRelease(KeyCode::MiddleMouse); break;
        case 3: Input::INS.GetKeyRelease(KeyCode::X1Mouse); break;
        case 4: Input::INS.GetKeyRelease(KeyCode::X2Mouse); break;
    }
}

void Game::CaptureMouseMovement(const POINTS* const mousePos, bool isClipped)
{ 
    Input::INS.mousePosition = Vector2Int(mousePos->x, mousePos->y); 
    Input::INS.clippedCursor = isClipped;
}
