#pragma once

typedef void (BaseComponent::*InputCallback)();
typedef void (BaseComponent::*InputCallbackFloat)(float);

struct InputCallbackContainer
{
	InputCallback callback;
	BaseComponent* component;

	bool operator==(InputCallbackContainer other) const 
	{ 
		return callback == other.callback && component == other.component; 
	}
};

struct InputCallbackFloatContainer
{
	KeyCode key1, key2;
	InputCallbackFloat callback;
	BaseComponent* component;

	bool operator==(InputCallbackFloatContainer other) const
	{
		return key1 == other.key1 && key2 == other.key2 && callback == other.callback && component == other.component;
	}
};

class Input
{
	friend class Game;

public:
	static Input INS;

	Input();

	template<class T>
	void RegisterAxis(std::string axisName, KeyCode key1, KeyCode key2, void (T::* callback)(float), T* const caller)
	{
		keyFloatCallbacks[axisName] = InputCallbackFloatContainer{ key1, key2, static_cast<InputCallbackFloat>(callback), caller };
	}

	template<class T>
	void RegisterKeyPress(KeyCode key, void (T::* callback)(), T* const caller)
	{
		if (!keyCallbacks.contains(key)) InitCallbackMapForKey(key);
		keyCallbacks[key][0].push_back(InputCallbackContainer{ static_cast<InputCallback>(callback), caller });
	}

	template<class T>
	void RegisterKeyRelease(KeyCode key, void (T::* callback)(), T* const caller)
	{
		if (!keyCallbacks.contains(key)) InitCallbackMapForKey(key);
		keyCallbacks[key][1].push_back(InputCallbackContainer{ static_cast<InputCallback>(callback), caller });
	}

	template<class T>
	void RegisterKeyHold(KeyCode key, void (T::* callback)(), T* const caller)
	{
		if (!keyCallbacks.contains(key)) InitCallbackMapForKey(key);
		keyCallbacks[key][2].push_back(InputCallbackContainer{ static_cast<InputCallback>(callback), caller });
	}

	template<class T>
	void UnRegisterKeyPress(KeyCode key, void (T::* callback)(), T* const caller)
	{
		InputCallback castedPointer = static_cast<InputCallback>(callback);
		std::vector<InputCallbackContainer>& vec = keyCallbacks[key][0];

		for (InputCallbackContainer container : vec)
		{
			if (container.callback != castedPointer || container.component != caller) continue;
			vec.erase(std::remove(vec.begin(), vec.end(), container));
		}
	}

	template<class T>
	void UnRegisterKeyRelease(KeyCode key, void (T::* callback)(), T* const caller)
	{
		InputCallback castedPointer = static_cast<InputCallback>(callback);
		std::vector<InputCallbackContainer>& vec = keyCallbacks[key][1];

		for (InputCallbackContainer container : vec)
		{
			if (container.callback != castedPointer || container.component != caller) continue;
			vec.erase(std::remove(vec.begin(), vec.end(), container));
		}
	}

	template<class T>
	void UnRegisterKeyHold(KeyCode key, void (T::* callback)(), T* const caller)
	{
		InputCallback castedPointer = static_cast<InputCallback>(callback);
		std::vector<InputCallbackContainer>& vec = keyCallbacks[key][2];

		for (InputCallbackContainer container : vec)
		{
			if (container.callback != castedPointer || container.component != caller) continue;
			vec.erase(std::remove(vec.begin(), vec.end(), container));
		}
	}

	template<class T>
	void UnRegisterAxis(std::string axisName, T* caller)
	{
		if (!keyFloatCallbacks.contains(axisName)) return;
		if (keyFloatCallbacks[axisName].component != caller) return;
		keyFloatCallbacks.erase(axisName);
	}

	Vector2Int GetMousePos() const { return mousePosition; }
	Vector2 GetMouseAxis() const { return mouseMov; }

private:
	std::map<KeyCode, std::vector<InputCallbackContainer>[3]> keyCallbacks;
	std::map<KeyCode, bool> currentKeys;
	std::map<std::string, InputCallbackFloatContainer> keyFloatCallbacks;

	bool clippedCursor;
	float senstivity;
	Vector2Int mousePosition;
	Vector2 mouseMov;

	void ProcessInputs();
	void GetKeyPress(const KeyCode& key);
	void GetKeyRelease(const KeyCode& key);
	void InitCallbackMapForKey(const KeyCode& key);
};
