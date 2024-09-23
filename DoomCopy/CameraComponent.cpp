#include "pch.h"
#include "GameObjects.h"
#include "Component.h"
#include "pch.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "World.h"
#include "Inputs.h"
#include "GameTime.h"

void Camera::Start()
{
	BaseComponent::Start();

	processedSectors = new ProcessedSector[5];
	for (int i = 0; i < 5; i++) processedSectors[i] = ProcessedSector();

	Input::INS.RegisterKeyHold(KeyCode::W, &Camera::DebugForward, this);
	Input::INS.RegisterKeyHold(KeyCode::S, &Camera::DebugBack, this);
	Input::INS.RegisterKeyHold(KeyCode::A, &Camera::DebugLeft, this);
	Input::INS.RegisterKeyHold(KeyCode::D, &Camera::DebugRight, this);
	Input::INS.RegisterKeyHold(KeyCode::Q, &Camera::DebugRotLeft, this);
	Input::INS.RegisterKeyHold(KeyCode::E, &Camera::DebugRotRight, this);
	Input::INS.RegisterKeyHold(KeyCode::R, &Camera::DebugRotUp, this);
	Input::INS.RegisterKeyHold(KeyCode::F, &Camera::DebugRotDown, this);
	Input::INS.RegisterKeyHold(KeyCode::Z, &Camera::DebugUp, this);
	Input::INS.RegisterKeyHold(KeyCode::X, &Camera::DebugDown, this);
}

const float Camera::movSpeed = 15.0f;
const float Camera::rotSpeed = 15.0f;

ProcessedSector* Camera::GetProcessedSectors() { return processedSectors; };

void Camera::Tick()
{
	int sectorsToProcess = GetSectorsToProcess();

	Vector3 currentPos = GetGameObject()->GetTransform()->GetPos();
	int currentRotation = (int) std::roundf(GetGameObject()->GetTransform()->GetRot());
	float cos = (float) SCTABLE.cos[currentRotation];
	float sin = (float) SCTABLE.sin[currentRotation];

	for (int s = 0; s < sectorsToProcess; s++)
	{
		processedSectors->avrgDistanceToCamera = 0;
		int walls = processedSectors->numberOfWalls;
		for (int w = 0; w < walls; w++)
		{
			ProcessedWall& wall = processedSectors[s].sectorWalls[w];
			wall.leftBtmPoint -= currentPos;
			wall.rightBtmPoint -= currentPos;
			processedSectors[s].bottomPoint -= currentPos.z;

			wall.leftBtmPoint = Vector3(wall.leftBtmPoint.x * cos - wall.leftBtmPoint.y * sin, wall.leftBtmPoint.y * cos + wall.leftBtmPoint.x * sin, processedSectors[s].bottomPoint);
			wall.rightBtmPoint = Vector3(wall.rightBtmPoint.x * cos - wall.rightBtmPoint.y * sin, wall.rightBtmPoint.y * cos + wall.rightBtmPoint.x * sin, processedSectors[s].bottomPoint);

			wall.leftBtmPoint.z += (xRotation * wall.leftBtmPoint.y / 32);
			wall.rightBtmPoint.z += (xRotation * wall.rightBtmPoint.y / 32);

			wall.leftTopPoint = Vector3(wall.leftBtmPoint.x, wall.leftBtmPoint.y, wall.leftBtmPoint.z + processedSectors[s].topPoint);
			wall.rightTopPoint = Vector3(wall.rightBtmPoint.x, wall.rightBtmPoint.y, wall.rightBtmPoint.z + processedSectors[s].topPoint);

			processedSectors->avrgDistanceToCamera += Vector2::Distance(V2_ZERO, Vector2((wall.leftBtmPoint.x + wall.rightBtmPoint.x) / 2, (wall.leftBtmPoint.y + wall.rightBtmPoint.y) / 2));
			if (wall.leftBtmPoint.y < 1 && wall.rightBtmPoint.y < 1) continue;

			if (wall.leftBtmPoint.y < 1)
			{
				ClipBehindCamera(wall.leftBtmPoint, wall.rightBtmPoint);
				ClipBehindCamera(wall.leftTopPoint, wall.rightTopPoint);
			}

			if (wall.rightBtmPoint.y < 1)
			{
				ClipBehindCamera(wall.rightBtmPoint, wall.leftBtmPoint);
				ClipBehindCamera(wall.rightTopPoint, wall.leftTopPoint);
			}
		}

		processedSectors->avrgDistanceToCamera /= walls;
	}
}

int Camera::GetSectorsToProcess()
{
	int size = world->numberOfSectors > 5 ? 5 : world->numberOfSectors;
	for (int s = 0; s < size; s++)
	{
		int walls = world->sectorData[s].numberOfWalls;
		if (processedSectors[s].sectorWalls) delete[] processedSectors[s].sectorWalls;

		processedSectors[s].sectorWalls = new ProcessedWall[walls];
		for (int w = 0; w < walls; w++)
		{
			processedSectors[s].sectorWalls[w] = ProcessedWall
			{
				V3_ZERO, V3_ZERO,
				world->sectorData[s].sectorWalls[w].leftPoint, 
				world->sectorData[s].sectorWalls[w].rightPoint, 
				world->sectorData[s].sectorWalls[w].color
			};
		}

		processedSectors[s].numberOfWalls = world->sectorData[s].numberOfWalls;
		processedSectors[s].bottomPoint = world->sectorData[s].bottomPoint;
		processedSectors[s].topPoint = world->sectorData[s].topPoint;
	}

	return size;
}

void Camera::OnDestroy()
{
	Input::INS.UnRegisterKeyHold(KeyCode::W, &Camera::DebugForward, this);
	Input::INS.UnRegisterKeyHold(KeyCode::S, &Camera::DebugBack, this);
	Input::INS.UnRegisterKeyHold(KeyCode::A, &Camera::DebugLeft, this);
	Input::INS.UnRegisterKeyHold(KeyCode::D, &Camera::DebugRight, this);
	Input::INS.UnRegisterKeyHold(KeyCode::Q, &Camera::DebugRotLeft, this);
	Input::INS.UnRegisterKeyHold(KeyCode::E, &Camera::DebugRotRight, this);
	Input::INS.UnRegisterKeyHold(KeyCode::R, &Camera::DebugRotUp, this);
	Input::INS.UnRegisterKeyHold(KeyCode::F, &Camera::DebugRotDown, this);
	Input::INS.UnRegisterKeyHold(KeyCode::Z, &Camera::DebugUp, this);
	Input::INS.UnRegisterKeyHold(KeyCode::X, &Camera::DebugDown, this);
}

void Camera::ClipBehindCamera(Vector3& pointA, const Vector3& pointB)
{
	float d = pointA.y - pointB.y;
	if (d == 0) d = 1;
	float s = pointA.y / (pointA.y - pointB.y);
	pointA.x += s * (pointB.x - pointA.x);
	pointA.y += s * (pointB.y - pointA.y);
	pointA.z += s * (pointB.z - pointA.z);
	if (pointA.y == 0) pointA.y = 1;
}

void Camera::DebugForward()
{
	Vector3 fwd = GetGameObject()->GetTransform()->GetForwardVector();
	GetGameObject()->GetTransform()->TeleportTo(fwd * movSpeed * Time::INS.GetFDeltaTime());
}

void Camera::DebugBack()
{
	Vector3 fwd = GetGameObject()->GetTransform()->GetForwardVector();
	GetGameObject()->GetTransform()->TeleportTo(-fwd * movSpeed * Time::INS.GetFDeltaTime());
}

void Camera::DebugLeft()
{
	Vector3 left = GetGameObject()->GetTransform()->GetLeftVector();
	GetGameObject()->GetTransform()->TeleportTo(left * movSpeed * Time::INS.GetFDeltaTime());

}

void Camera::DebugRight()
{
	Vector3 left = GetGameObject()->GetTransform()->GetLeftVector();
	GetGameObject()->GetTransform()->TeleportTo(-left * movSpeed * Time::INS.GetFDeltaTime());
}

void Camera::DebugRotRight() { GetGameObject()->GetTransform()->Rotate(rotSpeed * Time::INS.GetFDeltaTime()); }
void Camera::DebugRotLeft() { GetGameObject()->GetTransform()->Rotate(-rotSpeed * Time::INS.GetFDeltaTime()); }
void Camera::DebugRotUp() { xRotation -= (int) std::roundf(rotSpeed * Time::INS.GetFDeltaTime()); }
void Camera::DebugRotDown() { xRotation += (int) std::roundf(rotSpeed * Time::INS.GetFDeltaTime()); }
void Camera::DebugUp() { GetGameObject()->GetTransform()->TeleportTo(V3_UP * movSpeed * Time::INS.GetFDeltaTime()); }
void Camera::DebugDown() { GetGameObject()->GetTransform()->TeleportTo(V3_DOWN * movSpeed * Time::INS.GetFDeltaTime()); }
