#include "pch.h"
#include "GameObjects.h"
#include "Component.h"
#include "VectorMath.h"
#include "Inputs.h"

Input::Input()
{
	keyCallbacks = std::map<KeyCode, std::vector<InputCallbackContainer>[3]>();
}

void Input::ProcessInputs()
{
	for (std::pair<KeyCode, bool> pair : currentKeys)
	{
		if (!keyCallbacks.contains(pair.first)) continue;
		std::vector<InputCallbackContainer>& vec = keyCallbacks[pair.first][2];
		size_t vecSize = vec.size();
		for (size_t i = 0; i < vecSize; i++) (vec[i].component->*vec[i].callback)();
	}
}

void Input::GetKeyPress(KeyCode key)
{
	if (currentKeys.contains(key)) return;
	currentKeys[key] = true;

	if (!keyCallbacks.contains(key)) return;
	std::vector<InputCallbackContainer>& vec = keyCallbacks[key][0];
	size_t vecSize = vec.size();
	for (size_t i = 0; i < vecSize; i++) (vec[i].component->*vec[i].callback)();
}

void Input::GetKeyRelease(KeyCode key)
{
	if (!currentKeys.contains(key)) return;
	currentKeys.erase(key);

	if (!keyCallbacks.contains(key)) return;
	std::vector<InputCallbackContainer>& vec = keyCallbacks[key][1];
	size_t vecSize = vec.size();
	for (size_t i = 0; i < vecSize; i++) (vec[i].component->*vec[i].callback)();
}

void Input::InitCallbackMapForKey(KeyCode key)
{
	keyCallbacks[key][0] = std::vector<InputCallbackContainer>();
	keyCallbacks[key][1] = std::vector<InputCallbackContainer>();
	keyCallbacks[key][2] = std::vector<InputCallbackContainer>();
}
