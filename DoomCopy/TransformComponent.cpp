#include "pch.h"
#include "GameTime.h"
#include "GameObjects.h"
#include "Component.h"
#include "TransformComponent.h"

void Transform::Start()
{
	BaseComponent::Start();
}

void Transform::Tick()
{
	//OLOG_LF("Pos - ({0}, {1}, {2})", pos.x, pos.y, pos.z);
}

void Transform::OnDestroy()
{

}

Transform::Transform() : pos(), rot(0)
{ }

void Transform::SetPos(Vector3 newPos) { pos = newPos; }

void Transform::TeleportTo(Vector3 newPos) { pos += newPos; }

void Transform::Rotate(float angle)
{
	angle = std::clamp(angle, -360.0f, 360.0f);
	rot += angle;

	if (rot < 0) rot += 360;
	if (rot > 359) rot -= 360;
}
