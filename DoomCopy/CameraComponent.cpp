#include "pch.h"
#include "GameObjects.h"
#include "Component.h"
#include "pch.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "World.h"
#include "Inputs.h"

void Camera::Start()
{
	// For Debug!
}

void Camera::Tick()
{
	Vector3 fwd = GetGameObject()->GetTransform()->GetForwardVector();
	Vector3 right = GetGameObject()->GetTransform()->GetRightVector();

	switch (Input::INS.GetLastKeyCode())
	{
		case KeyCode::W:
			GetGameObject()->GetTransform()->TeleportTo(fwd * 10.0);
			break;

		case KeyCode::S:
			GetGameObject()->GetTransform()->TeleportTo(-fwd * 10.0);
			break;

		case KeyCode::A:
			GetGameObject()->GetTransform()->TeleportTo(-right * 10.0);
			break;

		case KeyCode::D:
			GetGameObject()->GetTransform()->TeleportTo(right * 10.0);
			break;

		case KeyCode::Q:
			GetGameObject()->GetTransform()->Rotate(-4);
			break;

		case KeyCode::E:
			GetGameObject()->GetTransform()->Rotate(4);
			break;
	}

	Vector3 currentPos = GetGameObject()->GetTransform()->GetPos();
	Vector3 a = world->wallData.leftBtmPoint - currentPos;
	Vector3 b = world->wallData.rightBtmPoint - currentPos;
	Vector3 c = world->wallData.leftTopPoint - currentPos;
	Vector3 d = world->wallData.rightTopPoint - currentPos;

	int currentRotation = (int) std::roundf(GetGameObject()->GetTransform()->GetRot());
	float cos = (float) SCTABLE.cos[currentRotation];
	float sin = (float) SCTABLE.sin[currentRotation];

	world->wall.leftBtmPoint = Vector3(a.x * cos - a.z * sin, a.y, a.z * cos + a.x * sin);
	world->wall.rightBtmPoint = Vector3(b.x * cos - b.z * sin, b.y, b.z * cos + b.x * sin);

	world->wall.leftTopPoint = Vector3(c.x * cos - c.z * sin, c.y, c.z * cos + c.x * sin);
	world->wall.rightTopPoint = Vector3(d.x * cos - d.z * sin, d.y, d.z * cos + d.x * sin);

	if (world->wall.leftBtmPoint.z < 1 && world->wall.rightBtmPoint.z < 1) return;

	if (world->wall.leftBtmPoint.z < 1)
	{
		ClipBehindCamera(world->wall.leftBtmPoint, world->wall.rightBtmPoint);
		ClipBehindCamera(world->wall.leftTopPoint, world->wall.rightTopPoint);
	}

	if (world->wall.rightBtmPoint.z < 1)
	{
		ClipBehindCamera(world->wall.rightBtmPoint, world->wall.leftBtmPoint);
		ClipBehindCamera(world->wall.rightTopPoint, world->wall.leftTopPoint);
	}
}

void Camera::OnDestroy()
{

}

void Camera::ClipBehindCamera(Vector3& pointA, const Vector3& pointB)
{
	float da = pointA.z;
	float db = pointB.z;
	float d = da - db;
	if (d == 0) d = 1;
	float s = da / (da - db);
	pointA.x += s * (pointB.x - pointA.x);
	pointA.y += s * (pointB.y - pointA.y);
	pointA.z += s * (pointB.z - pointA.z);
	if (pointA.z == 0) pointA.z = 1;
}

void Camera::DebugForward()
{

}

void Camera::DebugBack()
{

}

void Camera::DebugLeft()
{

}

void Camera::DebugRight()
{

}

void Camera::DebugRotRight()
{

}

void Camera::DebugRotLeft()
{

}
