#pragma once

enum KeyCode 
{ 
	None = 0,
	Backspace = VK_BACK, Tab = VK_TAB, Enter = VK_RETURN, Shift = VK_SHIFT, Ctrl = VK_CONTROL, Alt = VK_MENU, Pause = VK_PAUSE,
	CapsLock = VK_CAPITAL, Esc = VK_ESCAPE, Spacebar = VK_SPACE, PageUp = VK_PRIOR, PageDown = VK_NEXT, End = VK_END, Home = VK_HOME,
	Left = VK_LEFT, Up = VK_UP, Right = VK_RIGHT, Down = VK_DOWN, Select = VK_SELECT, Insert = VK_INSERT, Del = VK_DELETE,

	Number0 = 0x30, Number1 = 0x31, Number2 = 0x32, Number3 = 0x33, Number4 = 0x34, Number5 = 0x35, Number6 = 0x36, Number7 = 0x37,
	Number8 = 0x38, Number9 = 0x39,

	A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45, F = 0x46, G = 0x47, H = 0x48, I = 0x49, J = 0x4A, K = 0x4B, L = 0x4C, M = 0x4D,
	N = 0x4E, O = 0x4F, P = 0x50, Q = 0x51, R = 0x52, S = 0x53, T = 0x54, U = 0x55, V = 0x56, W = 0x57, X = 0x58, Y = 0x59, Z = 0x5A,

	Numpad0 = VK_NUMPAD0, Numpad1 = VK_NUMPAD1, Numpad2 = VK_NUMPAD2, Numpad3 = VK_NUMPAD3, Numpad4 = VK_NUMPAD4, Numpad5 = VK_NUMPAD5,
	Numpad6 = VK_NUMPAD6, Numpad7 = VK_NUMPAD7, Numpad8 = VK_NUMPAD8, Numpad9 = VK_NUMPAD9,

	Mult = VK_MULTIPLY, Add = VK_ADD, Sep = VK_SEPARATOR, Subs = VK_SUBTRACT, Decimal = VK_DECIMAL, Divide = VK_DIVIDE,

	F1 = VK_F1, F2 = VK_F2, F3 = VK_F3, F4 = VK_F4, F5 = VK_F5, F6 = VK_F6, F7 = VK_F7, F8 = VK_F8, F9 = VK_F9, F10 = VK_F10, F11 = VK_F11,
	F12 = VK_F12,

	LeftMouse = 0x01, RightMouse = 0x02, MiddleMouse = 0x03, X1Mouse = 0x04, X2Mouse = 0x05
};

typedef void (BaseComponent::*InputCallback)();

struct InputCallbackContainer
{
	InputCallback callback;
	BaseComponent* component;

	bool operator==(InputCallbackContainer other) const 
	{ 
		return callback == other.callback && component == other.component; 
	}
};

class Input
{
	friend class Game;

public:
	static Input INS;

	Input();

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

private:
	std::map<KeyCode, std::vector<InputCallbackContainer>[3]> keyCallbacks;
	std::map<KeyCode, bool> currentKeys;
	Vector2 mousePosition;

	void ProcessInputs();
	void GetKeyPress(KeyCode key);
	void GetKeyRelease(KeyCode key);
	void InitCallbackMapForKey(KeyCode key);
};
