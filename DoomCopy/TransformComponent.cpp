#include "framework.h"
#include "GameTime.h"
#include "Component.h"
#include "TransformComponent.h"

void Transform::Start()
{
	BaseComponent::Start();
	pos.x = 200;
	pos.y = 300;
	OLOG_L("Start!");
}

void Transform::Tick()
{
	pos.y++;
	if (pos.y > 400) pos.y = 300;
	OLOG_LF("Ticking! - {0}", Time::INS.GetDeltaTime());
}

void Transform::OnDestroy()
{

}

Transform::Transform() : pos(), rot(0)
{ }
