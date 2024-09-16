#include "framework.h"
#include "Component.h"
#include "TransformComponent.h"

void Transform::Start()
{
	BaseComponent::Start();
	x = 200;
	y = 300;
	OLOG_L("Start!");
}

void Transform::Tick()
{
	y++;
	if (y > 400) y = 300;
	OLOG_L("Ticking!");
}

void Transform::OnDestroy()
{

}
