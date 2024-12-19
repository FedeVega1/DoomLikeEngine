#include "pch.h"
#include "GameObjects.h"
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "World.h"
#include "Inputs.h"
#include "GameTime.h"

void Camera::Start()
{
	BaseComponent::Start();

	closestNodes = new BSPNode*[world->maxNumberOfBSPNodes];
	processedWalls = nullptr;
	processedSectors = nullptr;

	for (int i = 0; i < world->maxNumberOfBSPNodes; i++)
		closestNodes[i] = nullptr;

	Input::INS.RegisterAxis("CameraForwardBack", KeyCode::W, KeyCode::S, &Camera::DebugForwardBack, this);
	Input::INS.RegisterAxis("CameraLeftRight", KeyCode::A, KeyCode::D, &Camera::DebugLeftRight, this);
	Input::INS.RegisterAxis("CameraUpDown", KeyCode::Z, KeyCode::X, &Camera::DebugUpDown, this);
	Input::INS.RegisterAxis("CameraRotUpDown", KeyCode::R, KeyCode::F, &Camera::DebugRotUpDown, this);
}

const float Camera::movSpeed = 400;
const float Camera::rotSpeed = 350;

int Camera::GetProcessedWalls(ProcessedWall*& outProcessedWalls) const
{ 
	outProcessedWalls = processedWalls;
	return numbProcessedWalls;
}

void Camera::Tick()
{
	GetTransform()->Rotate(rotSpeed * Input::INS.GetMouseAxis().x * Time::TimeSlice);
}

void Camera::AfterTick()
{
	GetSectorsToProcess();

	Vector3 currentPos = GetTransform()->GetPos();
	Vector2 posToSplitter = currentPos.XY() - world->splitterWall.leftPoint;
	numbProcessedWalls = 0;

	GetWallsFromBSP(posToSplitter, &world->rootNode, closestNodes, numbProcessedWalls);

	if (!closestNodes || !closestNodes[0])
	{
		OLOG_E("Can't find the camera anywhere inside the level!");
		return;
	}

	if (processedWalls) delete[] processedWalls;
	processedWalls = new ProcessedWall[numbProcessedWalls];

	currentPos.z += cameraZOffset;
	currentPos.z *= -1;

	int currentRotation = (int) std::roundf(GetTransform()->GetRot());
	float cos = (float) SCTABLE.cos[currentRotation];
	float sin = (float) SCTABLE.sin[currentRotation];

	for (int i = 0; i < numbProcessedWalls; i++)
	{
		int w, s = closestNodes[i]->sectorIndx;
		if (!world->FindWallByIDWithSector(closestNodes[i]->wall.wallID, s, w)) continue;

		ProcessedWall& wall = processedWalls[i] = ProcessedWall
		{
			V3_ZERO, V3_ZERO,
			world->sectorData[s].sectorWalls[w].leftPoint,
			world->sectorData[s].sectorWalls[w].rightPoint,
			world->sectorData[s].sectorWalls[w].topColor,
			world->sectorData[s].sectorWalls[w].inColor,
			world->sectorData[s].sectorWalls[w].btmColor,
			world->sectorData[s].sectorWalls[w].isPortal,
			world->sectorData[s].sectorWalls[w].isConnection,
			world->sectorData[s].sectorWalls[w].portalTargetSector,
			world->sectorData[s].sectorWalls[w].portalTargetWall,
			s
		};

		wall.leftBtmPoint.AddXY(-currentPos.x, -currentPos.y);
		wall.rightBtmPoint.AddXY(-currentPos.x, -currentPos.y);

		wall.leftBtmPoint = Vector3((wall.leftBtmPoint.x * cos) - (wall.leftBtmPoint.y * sin), (wall.leftBtmPoint.y * cos) + (wall.leftBtmPoint.x * sin), world->sectorData[s].bottomPoint + currentPos.z);
		wall.rightBtmPoint = Vector3((wall.rightBtmPoint.x * cos) - (wall.rightBtmPoint.y * sin), (wall.rightBtmPoint.y * cos) + (wall.rightBtmPoint.x * sin), world->sectorData[s].bottomPoint + currentPos.z);

		wall.leftBtmPoint.z += (xRotation * wall.leftBtmPoint.y / 32.0f);
		wall.rightBtmPoint.z += (xRotation * wall.rightBtmPoint.y / 32.0f);

		wall.leftTopPoint = Vector3(wall.leftBtmPoint.x, wall.leftBtmPoint.y, world->sectorData[s].topPoint + currentPos.z);
		wall.rightTopPoint = Vector3(wall.rightBtmPoint.x, wall.rightBtmPoint.y, world->sectorData[s].topPoint + currentPos.z);

		wall.leftTopPoint.z += (xRotation * wall.leftBtmPoint.y / 32.0f);
		wall.rightTopPoint.z += (xRotation * wall.rightBtmPoint.y / 32.0f);

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

void Camera::GetWallsFromBSP(const Vector2& pos, BSPNode* startNode, BSPNode** closestNodesArray, int& outArraySize)
{
	if (!startNode) return;

	if (world->splitterWall.VectorInFrontWall(pos))
	{
		GetWallsFromBSP(pos, startNode->frontNode, closestNodesArray, outArraySize);
		GetWallsFromBSP(pos, startNode->backNode, closestNodesArray, outArraySize);
		return;
	}

	GetWallsFromBSP(pos, startNode->backNode, closestNodesArray, outArraySize);
	closestNodesArray[outArraySize++] = startNode;
	GetWallsFromBSP(pos, startNode->frontNode, closestNodesArray, outArraySize);
}

void Camera::GetSectorsToProcess()
{
	numbProcessedSectors = world->numberOfSectors;

	if (!processedSectors) processedSectors = std::make_shared<ProcessedSector[]>(numbProcessedSectors);
	else 
	{
		for (int i = 0; i < numbProcessedSectors; i++) 
		{
			delete[] processedSectors[i].sectorWalls;
			processedSectors[i] = ProcessedSector();
		}
	}

	for (int i = GetClosestWorldSector(), s = 0; i < numbProcessedSectors; i++)
		ProcessWorldSector(s, i, -1);
}

void Camera::ProcessWorldSector(int& sector, int worldSector, int portalIndx)
{
	if (!world->sectorData[worldSector].HasPortals())
	{
		if (CheckIfSectorIsProcessed(worldSector)) return;
		CopyWorldSectorData(sector, worldSector);
		return;
	}

	std::vector<int> portalConnections = std::vector<int>();
	world->sectorData[worldSector].GetPortalSectors(&portalConnections, portalIndx);
	for (int i = 0; i < portalConnections.size(); i++)
	{
		if (CheckIfSectorIsProcessed(portalConnections[i])) continue;
		ProcessWorldSector(sector, portalConnections[i], worldSector);
	}

	if (CheckIfSectorIsProcessed(worldSector)) return;
	CopyWorldSectorData(sector, worldSector);
}

bool Camera::CheckIfSectorIsProcessed(int sector)
{
	for (int i = 0; i < numbProcessedSectors; i++)
	{
		if (processedSectors[i].worldSectorIndex != sector) continue;
		return true;
	}

	return false;
}

void Camera::CopyWorldSectorData(int& sector, int worldSector)
{
	int walls = world->sectorData[worldSector].numberOfWalls;
	if (processedSectors[sector].sectorWalls != nullptr) delete[] processedSectors[sector].sectorWalls;

	processedSectors[sector].sectorWalls = new ProcessedWall[walls];
	for (int w = 0; w < walls; w++)
	{
		//processedSectors[sector].sectorWalls[w] = ProcessedWall
		//{
		//	V3_ZERO, V3_ZERO,
		//	world->sectorData[worldSector].sectorWalls[w].leftPoint,
		//	world->sectorData[worldSector].sectorWalls[w].rightPoint,
		//	world->sectorData[worldSector].sectorWalls[w].topColor,
		//	world->sectorData[worldSector].sectorWalls[w].inColor,
		//	world->sectorData[worldSector].sectorWalls[w].btmColor,
		//	world->sectorData[worldSector].sectorWalls[w].isPortal,
		//	world->sectorData[worldSector].sectorWalls[w].isConnection,
		//	world->sectorData[worldSector].sectorWalls[w].portalTargetSector,
		//	world->sectorData[worldSector].sectorWalls[w].portalTargetWall
		//};
	}

	processedSectors[sector].numberOfWalls = walls;
	processedSectors[sector].bottomPoint = world->sectorData[worldSector].bottomPoint;
	processedSectors[sector].topPoint = world->sectorData[worldSector].topPoint;
	processedSectors[sector].floorColor = world->sectorData[worldSector].floorColor;
	processedSectors[sector].ceillingColor = world->sectorData[worldSector].ceillingColor;
	processedSectors[sector].sectorID = world->sectorData[worldSector].sectorID;
	processedSectors[sector++].worldSectorIndex = worldSector;
}

int Camera::GetClosestWorldSector()
{
	int overlapSectorsSize = 0, size = world->numberOfSectors, sector = 0;
	int* overlapSectors = new int[size];
	Vector2 currentPos = GetTransform()->GetPos().XY();

	for (int i = 0; i < size; i++)
	{
		if (!world->sectorData[i].PointIsInsideSector(currentPos)) continue;
		overlapSectors[overlapSectorsSize++] = i;
	}

	if (overlapSectorsSize == 1)
	{
		sector = overlapSectors[0];
		delete[] overlapSectors;
		return sector;
	}

	float closestDistance = 99999999.0f;
	for (int i = 0; i < overlapSectorsSize; i++)
	{
		float dist = world->sectorData[overlapSectors[i]].GetAvrgDistanceToPoint(currentPos);
		if (dist >= closestDistance) continue;
		closestDistance = dist;
		sector = overlapSectors[i];
	}

	delete[] overlapSectors;
	return sector;
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

	if (pointA.y < 1) pointA.y = 1;
	else if (pointA.y > 1) pointA.y = -1;
}

void Camera::DebugForwardBack(float axis)
{
	Vector3 fwd = GetTransform()->GetForwardVector();
	GetTransform()->TeleportTo(fwd * axis * movSpeed * Time::TimeSlice);
}

void Camera::DebugLeftRight(float axis)
{
	Vector3 left = GetTransform()->GetLeftVector();
	GetTransform()->TeleportTo(left * axis * movSpeed * Time::TimeSlice);
}

void Camera::DebugUpDown(float axis)
{
	GetTransform()->TeleportTo(V3_UP * axis * movSpeed * Time::TimeSlice);
}

void Camera::DebugRotUpDown(float axis)
{
	xRotation -= (int) std::roundf(rotSpeed * axis * Time::TimeSlice);
}
