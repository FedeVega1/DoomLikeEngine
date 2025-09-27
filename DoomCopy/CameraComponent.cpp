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

	processedWalls = std::vector<ProcessedWall>();
	processedWalls.reserve(10);

	Input::INS.RegisterAxis("CameraForwardBack", KeyCode::W, KeyCode::S, &Camera::DebugForwardBack, this);
	Input::INS.RegisterAxis("CameraLeftRight", KeyCode::A, KeyCode::D, &Camera::DebugLeftRight, this);
	Input::INS.RegisterAxis("CameraUpDown", KeyCode::Z, KeyCode::X, &Camera::DebugUpDown, this);
	Input::INS.RegisterAxis("CameraRotUpDown", KeyCode::R, KeyCode::F, &Camera::DebugRotUpDown, this);
}

const float Camera::movSpeed = 400;
const float Camera::rotSpeed = 350;

void Camera::GetProcessedWalls(std::vector<ProcessedWall>& outProcessedWalls) const { outProcessedWalls = processedWalls; }

void Camera::Tick()
{
	GetTransform()->Rotate(rotSpeed * Input::INS.GetMouseAxis().x * Time::TimeSlice);
}

void Camera::AfterTick()
{
	processedWalls.clear();
	Vector3 currentPos = GetTransform()->GetPos();

	int currentRotation = (int) std::roundf(GetTransform()->GetRot());
	float cos = (float) SCTABLE.cos[currentRotation];
	float sin = (float) SCTABLE.sin[currentRotation];

	GetWallsFromBSP(currentPos, world->rootNode, cos, sin);
	if (processedWalls.size() == 0) OLOG_E("Can't find the camera anywhere inside the level!");
}

void Camera::GetWallsFromBSP(const Vector3& pos, BSPNode* startNode, const float& cos, const float& sin)
{
	if (!startNode) return;

	if (startNode->IsLeafNode())
	{
		ProcessSubSectorFromBSPNode(startNode->subSector, pos, cos, sin);
		return;
	}

	//Vector2 posToSplitter = pos.XY() - startNode->splitter.startPoint;

	if (startNode->splitter.VectorInFront(pos.XY()))
	{
		GetWallsFromBSP(pos, startNode->backNode, cos, sin);
		GetWallsFromBSP(pos, startNode->frontNode, cos, sin);
		return;
	}

	GetWallsFromBSP(pos, startNode->backNode, cos, sin);
	GetWallsFromBSP(pos, startNode->frontNode, cos, sin);
}

void Camera::ProcessSubSectorFromBSPNode(const SubSector* const subSector, Vector3 pos, const float& cos, const float& sin)
{
	pos.z += cameraZOffset;
	pos.z *= -1;

	for (size_t i = 0; i < subSector->subSectorWalls.size(); i++)
	{
		ProcessedWall wall = ProcessedWall
		{
			V3_ZERO, V3_ZERO,
			subSector->subSectorWalls[i].leftPoint, subSector->subSectorWalls[i].rightPoint,
			subSector->subSectorWalls[i].topColor, subSector->subSectorWalls[i].inColor, subSector->subSectorWalls[i].btmColor,
			subSector->subSectorWalls[i].isPortal, subSector->subSectorWalls[i].isConnection,
			subSector->subSectorWalls[i].portalTargetSectorID, subSector->subSectorWalls[i].portalTargetWall,
			subSector->subSectorWalls[i].parentSector
		};

		wall.leftBtmPoint.AddXY(-pos.x, -pos.y);
		wall.rightBtmPoint.AddXY(-pos.x, -pos.y);

		wall.leftBtmPoint = Vector3((wall.leftBtmPoint.x * cos) - (wall.leftBtmPoint.y * sin), (wall.leftBtmPoint.y * cos) + (wall.leftBtmPoint.x * sin), wall.parentSector->bottomPoint + pos.z);
		wall.rightBtmPoint = Vector3((wall.rightBtmPoint.x * cos) - (wall.rightBtmPoint.y * sin), (wall.rightBtmPoint.y * cos) + (wall.rightBtmPoint.x * sin), wall.parentSector->bottomPoint + pos.z);

		wall.leftBtmPoint.z += (xRotation * wall.leftBtmPoint.y / 32.0f);
		wall.rightBtmPoint.z += (xRotation * wall.rightBtmPoint.y / 32.0f);

		wall.leftTopPoint = Vector3(wall.leftBtmPoint.x, wall.leftBtmPoint.y, wall.parentSector->topPoint + pos.z);
		wall.rightTopPoint = Vector3(wall.rightBtmPoint.x, wall.rightBtmPoint.y, wall.parentSector->topPoint + pos.z);

		wall.leftTopPoint.z += (xRotation * wall.leftBtmPoint.y / 32.0f);
		wall.rightTopPoint.z += (xRotation * wall.rightBtmPoint.y / 32.0f);

		if (wall.leftBtmPoint.y < 1 && wall.rightBtmPoint.y < 1)
		{
			//processedWalls.push_back(wall);
			//processedWalls.insert(processedWalls.begin(), wall);
			return;
		}

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

		processedWalls.push_back(wall);
		//processedWalls.insert(processedWalls.begin(), wall);
	}
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
