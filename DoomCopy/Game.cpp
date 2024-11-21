#include "pch.h"
#include "Game.h"
#include "CameraComponent.h"
#include "GameTime.h"
#include "TransformComponent.h"
#include "Inputs.h"
#include "World.h"

Time Time::INS = Time();
Input Input::INS = Input();

const float Time::TimeSlice = .016f;

void Game::InitUpdate()
{
    world = new World(this, mapToOpen);
    entities.push_back(world);

    GameObject* cameraObject = new GameObject(this);
    mainCamera = cameraObject->AddComponent<Camera>();
    mainCamera->world = world;
    entities.push_back(cameraObject);

    //cameraObject->GetTransform()->SetPos(Vector3(0, 0, 0));
    mainCamera->SetCameraZOffset(45);
}

void Game::MainUpdate()
{
    size_t count = entities.size();
    for (size_t i = 0; i < count; i++) entities[i]->PreTickComponents();

    long long currentTime = Time::INS.GetGameTickCount();
    while (Time::INS.simulationTime < currentTime)
    {
        // GameLoop
        Input::INS.ProcessInputs();

        for (size_t i = 0; i < count; i++) entities[i]->TickComponents();
        Time::INS.simulationTime += Time::INS.fixedTimeSlice;
    }

    for (size_t i = 0; i < count; i++) entities[i]->AfterTickComponents();
}

Game::Game() : mainCamera(nullptr), world(nullptr)
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

Sector* Game::GetSector(int sectorIndx) const { return &world->sectorData[sectorIndx]; }
Vector2 Game::GetSectorCenter(int sectorIndx) const { return GetSector(sectorIndx)->sectorCenter; }
float Game::GetSectorFloorHeight(int sectorIndx) const { return GetSector(sectorIndx)->bottomPoint; }
