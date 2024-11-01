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

	processedSectors = new ProcessedSector[Camera::numberOfSectorsToProcess];
	for (int i = 0; i < Camera::numberOfSectorsToProcess; i++) processedSectors[i] = ProcessedSector();

	Input::INS.RegisterAxis("CameraForwardBack", KeyCode::W, KeyCode::S, &Camera::DebugForwardBack, this);
	Input::INS.RegisterAxis("CameraLeftRight", KeyCode::A, KeyCode::D, &Camera::DebugLeftRight, this);
	Input::INS.RegisterAxis("CameraUpDown", KeyCode::Z, KeyCode::X, &Camera::DebugUpDown, this);
	Input::INS.RegisterAxis("CameraRotUpDown", KeyCode::R, KeyCode::F, &Camera::DebugRotUpDown, this);
}

const float Camera::movSpeed = 15.0f;
const float Camera::rotSpeed = 10.0f;

int Camera::GetProcessedSectors(const ProcessedSector** outProcessedSectors) 
{ 
	*outProcessedSectors = processedSectors;
	return numbProcessedSectors;
}

void Camera::Tick()
{
	GetTransform()->Rotate(rotSpeed * Input::INS.GetMouseAxis().x * Time::INS.GetFDeltaTime());

	numbProcessedSectors = GetSectorsToProcess();

	Vector3 currentPos = GetTransform()->GetPos();
	currentPos.z += cameraZOffset;

	int currentRotation = (int) std::roundf(GetTransform()->GetRot());
	float cos = (float) SCTABLE.cos[currentRotation];
	float sin = (float) SCTABLE.sin[currentRotation];

	for (int s = 0; s < numbProcessedSectors; s++)
	{
		processedSectors[s].avrgDistanceToCamera = 0;

		if ((currentPos.z - processedSectors[s].bottomPoint) < processedSectors[s].bottomPoint) processedSectors[s].surface = SectorSurface::Below;
		else if ((currentPos.z + processedSectors[s].topPoint) > processedSectors[s].topPoint) processedSectors[s].surface = SectorSurface::Above;
		else processedSectors[s].surface = SectorSurface::SurfNone;

		int walls = processedSectors->numberOfWalls;
		for (int w = 0; w < walls; w++)
		{
			int cycles = processedSectors[s].surface != SectorSurface::SurfNone ? 2 : 1;
			for (int i = 0; i < cycles; i++)
			{
				ProcessedWall& wall = processedSectors[s].sectorWalls[(w * 2) + i];

				wall.leftBtmPoint.AddXY(-currentPos.x, -currentPos.y);
				wall.rightBtmPoint.AddXY(-currentPos.x, -currentPos.y);

				if (i == 0)
				{
					Vector3 swp = wall.leftBtmPoint;
					wall.leftBtmPoint = wall.rightBtmPoint;
					wall.rightBtmPoint = swp;
				}

				wall.leftBtmPoint = Vector3((wall.leftBtmPoint.x * cos) - (wall.leftBtmPoint.y * sin), (wall.leftBtmPoint.y * cos) + (wall.leftBtmPoint.x * sin), processedSectors[s].bottomPoint + currentPos.z);
				wall.rightBtmPoint = Vector3((wall.rightBtmPoint.x * cos) - (wall.rightBtmPoint.y * sin), (wall.rightBtmPoint.y * cos) + (wall.rightBtmPoint.x * sin), processedSectors[s].bottomPoint + currentPos.z);

				wall.leftBtmPoint.z += (xRotation * wall.leftBtmPoint.y / 32.0f);
				wall.rightBtmPoint.z += (xRotation * wall.rightBtmPoint.y / 32.0f);

				wall.leftTopPoint = Vector3(wall.leftBtmPoint.x, wall.leftBtmPoint.y, processedSectors[s].topPoint + currentPos.z);
				wall.rightTopPoint = Vector3(wall.rightBtmPoint.x, wall.rightBtmPoint.y, processedSectors[s].topPoint + currentPos.z);

				wall.leftTopPoint.z += (xRotation * wall.leftBtmPoint.y / 32.0f);
				wall.rightTopPoint.z += (xRotation * wall.rightBtmPoint.y / 32.0f);

				processedSectors[s].avrgDistanceToCamera += Vector2::Distance(V2_ZERO, Vector2((wall.leftBtmPoint.x + wall.rightBtmPoint.x) / 2.0f, (wall.leftBtmPoint.y + wall.rightBtmPoint.y) / 2.0f));
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
		}

		processedSectors[s].avrgDistanceToCamera /= walls;
	}

	OrderSectorsByDistance();
}

int Camera::GetSectorsToProcess()
{
	int size = world->numberOfSectors > Camera::numberOfSectorsToProcess ? Camera::numberOfSectorsToProcess : world->numberOfSectors;
	for (int s = 0; s < size; s++)
	{
		int walls = world->sectorData[s].numberOfWalls;
		if (processedSectors[s].sectorWalls != nullptr) delete[] processedSectors[s].sectorWalls;

		processedSectors[s].sectorWalls = new ProcessedWall[walls * 2];
		for (int w = 0, i = 0; w < walls; i++)
		{
			processedSectors[s].sectorWalls[i] = ProcessedWall
			{
				V3_ZERO, V3_ZERO,
				world->sectorData[s].sectorWalls[w].leftPoint, 
				world->sectorData[s].sectorWalls[w].rightPoint, 
				world->sectorData[s].sectorWalls[w].color
			};

			if (i != 0 && i % 2 != 0) w++;
		}

		processedSectors[s].numberOfWalls = world->sectorData[s].numberOfWalls;
		processedSectors[s].bottomPoint = world->sectorData[s].bottomPoint;
		processedSectors[s].topPoint = world->sectorData[s].topPoint;
		processedSectors[s].floorColor = world->sectorData[s].floorColor;
		processedSectors[s].ceillingColor = world->sectorData[s].ceillingColor;
	}

	return size;
}

void Camera::OnDestroy()
{
	Input::INS.UnRegisterAxis("CameraForwardBack", this);
	Input::INS.UnRegisterAxis("CameraRightLeft", this);
	Input::INS.UnRegisterAxis("CameraUpDown", this);
	Input::INS.UnRegisterAxis("CameraRotUpDown", this);
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

void Camera::OrderSectorsByDistance()
{
	int sectors = numbProcessedSectors - 1;
	for (int i = 0; i < sectors; i++)
	{
		for (int j = 0; j < sectors - i; j++)
		{
			if (processedSectors[j].avrgDistanceToCamera >= processedSectors[j + 1].avrgDistanceToCamera) continue;
			const ProcessedSector sector = processedSectors[j];
			processedSectors[j] = processedSectors[j + 1];
			processedSectors[j + 1] = sector;
		}
	}
}

void Camera::DebugForwardBack(float axis)
{
	Vector3 fwd = GetTransform()->GetForwardVector();
	GetTransform()->TeleportTo(fwd * axis * movSpeed * Time::INS.GetFDeltaTime());
}

void Camera::DebugLeftRight(float axis)
{
	Vector3 left = GetTransform()->GetLeftVector();
	GetTransform()->TeleportTo(left * axis * movSpeed * Time::INS.GetFDeltaTime());
}

void Camera::DebugUpDown(float axis)
{
	GetTransform()->TeleportTo(V3_UP * axis * movSpeed * Time::INS.GetFDeltaTime());
}

void Camera::DebugRotUpDown(float axis)
{
	xRotation -= (int) std::roundf(rotSpeed * axis * Time::INS.GetFDeltaTime());
}
