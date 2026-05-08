#include "pch.h"
#include "GameTime.h"
#include "GameObjects.h"
#include "Game.h"
#include "World.h"
#include "TransformComponent.h"

void Transform::Start()
{
	BaseComponent::Start();

	if (currentSector != -1) return;
	if (!GetGameObject()->GetGameRef()->GetWorldRef()->CheckIfPositionInsideSector(pos, &currentSector))
		OLOG_EF("GameObject is out of bounds! - {0}", pos.ToString());
	else
		OLOG_LF("GameObject on sector: {0}", currentSector);
}

void Transform::Tick()
{
	//if (pos != lastPos)
	//{
	//	World* world = GetGameObject()->GetGameRef()->GetWorldRef();
	//	if (!world->CheckIfPositionInsideSector(pos, currentSector))
	//	{
	//		if (!world->CheckIfPositionInsideSector(pos, &currentSector))
	//			OLOG_EF("GameObject went out of Bounds! - {0}", pos.ToString());
	//		else
	//			OLOG_LF("GameObject moved to sector: {0}", currentSector);
	//	}
	//}

	lastPos = pos;
	//OLOG_LF("Pos - ({0}, {1}, {2})", pos.x, pos.y, pos.z);
}

void Transform::OnDestroy()
{

}

Transform::Transform() : pos(), rot(0), currentSector(-1)
{ }

void Transform::SetPos(const Vector3& newPos) { pos = newPos; }

void Transform::TeleportTo(const Vector3& newPos) { pos += newPos; }

void Transform::Rotate(float angle)
{
	angle = std::clamp(angle, -360.0f, 360.0f);
	rot += angle;

	if (rot < 0) rot += 360;
	if (rot > 359) rot -= 360;
}
