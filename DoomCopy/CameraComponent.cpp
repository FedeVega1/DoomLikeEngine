#include "pch.h"
#include "GameObjects.h"
#include "Component.h"
#include "pch.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
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
}

void Camera::OnDestroy()
{

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
