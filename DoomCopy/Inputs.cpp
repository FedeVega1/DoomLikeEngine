#include "pch.h"
#include "GameObjects.h"
#include "Component.h"
#include "VectorMath.h"
#include "Inputs.h"

Input::Input() : clippedCursor(false), senstivity(.4f)
{
	keyCallbacks = std::map<KeyCode, std::vector<InputCallbackContainer>[3]>();
	keyFloatCallbacks = std::map<std::string, InputCallbackFloatContainer>();
}

void Input::ProcessInputs()
{
	if (!clippedCursor) mouseMov = V2_ZERO;
	else mouseMov = Vector2::Clamp(Vector2(mousePosition - lastMousePosition) * senstivity, -1.0f, 1.0f);

	for (std::pair<KeyCode, bool> pair : currentKeys)
	{
		if (!keyCallbacks.contains(pair.first)) continue;
		std::vector<InputCallbackContainer>& vec = keyCallbacks[pair.first][2];
		size_t vecSize = vec.size();
		for (size_t i = 0; i < vecSize; i++) (vec[i].component->*vec[i].callback)();
	}

	for (std::pair<std::string, InputCallbackFloatContainer> pair : keyFloatCallbacks)
	{
		bool key1 = currentKeys.contains(pair.second.key1);
		bool key2 = currentKeys.contains(pair.second.key2);
		
		if (!key1 && !key2) (pair.second.component->*pair.second.callback)(0.0f);
		else if (key1 && key2) (pair.second.component->*pair.second.callback)(0.0f);
		else if (key1 && !key2) (pair.second.component->*pair.second.callback)(1.0f);
		else if (!key1 && key2) (pair.second.component->*pair.second.callback)(-1.0f);
	}

	lastMousePosition = mousePosition;
}

void Input::GetKeyPress(const KeyCode& key)
{
	if (currentKeys.contains(key)) return;
	currentKeys[key] = true;

	if (!keyCallbacks.contains(key)) return;
	std::vector<InputCallbackContainer>& vec = keyCallbacks[key][0];
	size_t vecSize = vec.size();
	for (size_t i = 0; i < vecSize; i++) (vec[i].component->*vec[i].callback)();
}

void Input::GetKeyRelease(const KeyCode& key)
{
	if (!currentKeys.contains(key)) return;
	currentKeys.erase(key);

	if (!keyCallbacks.contains(key)) return;
	std::vector<InputCallbackContainer>& vec = keyCallbacks[key][1];
	size_t vecSize = vec.size();
	for (size_t i = 0; i < vecSize; i++) (vec[i].component->*vec[i].callback)();
}

void Input::InitCallbackMapForKey(const KeyCode& key)
{
	keyCallbacks[key][0] = std::vector<InputCallbackContainer>();
	keyCallbacks[key][1] = std::vector<InputCallbackContainer>();
	keyCallbacks[key][2] = std::vector<InputCallbackContainer>();
}
