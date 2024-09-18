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
	F12 = VK_F12
};

class Component;
typedef void (Component::*InputCallback)();

typedef struct
{
	InputCallback callback;
	Component* component;
} InputCallbackContainer;

class Input
{
	friend class Game;

public:
	static Input INS;

	Input() : lastPressedKey(KeyCode::None), lastReleasedKey(KeyCode::None)
	{
		//keyCallbacks = std::map<KeyCode, std::vector<InputCallbackContainer>[2]>();
	}

	//void RegisterKeyPress(KeyCode key, InputCallback callback, Component* const caller)
	//{
	//	if (!keyCallbacks.contains(key))
	//	{
	//		keyCallbacks[key][0] = std::vector<InputCallbackContainer>();
	//		keyCallbacks[key][1] = std::vector<InputCallbackContainer>();
	//	}

	//	keyCallbacks[key][0].push_back(InputCallbackContainer{ callback, caller });
	//}

	//void RegisterKeyRelease(KeyCode key, InputCallback callback, Component* const caller)
	//{
	//	if (!keyCallbacks.contains(key))
	//	{
	//		keyCallbacks[key][0] = std::vector<InputCallbackContainer>();
	//		keyCallbacks[key][1] = std::vector<InputCallbackContainer>();
	//	}

	//	keyCallbacks[key][1].push_back(InputCallbackContainer{ callback, caller });
	//}

	//void UnregisterKeyPress(KeyCode key, InputCallback callback, Component* const caller)
	//{
	//	std::vector<InputCallbackContainer>& vec = keyCallbacks[key][0];

	//	for (InputCallbackContainer container : vec)
	//	{
	//		if (container.callback != callback || container.component != caller) continue;
	//		vec.erase(std::remove(vec.begin(), vec.end(), container));
	//	}
	//}

	//void UnregisterKeyRelease(KeyCode key, InputCallback callback, Component* const caller)
	//{
	//	std::vector<InputCallbackContainer>& vec = keyCallbacks[key][1];

	//	for (InputCallbackContainer container : vec)
	//	{
	//		if (container.callback != callback || container.component != caller) continue;
	//		vec.erase(std::remove(vec.begin(), vec.end(), container));
	//	}
	//}

	KeyCode GetLastKeyCode() const { return lastPressedKey; }

private:
	//std::map<KeyCode, std::vector<InputCallbackContainer>[2]> keyCallbacks;
	KeyCode lastPressedKey, lastReleasedKey;

	void GetKeyPress(KeyCode key)
	{
		lastPressedKey = key;
		lastReleasedKey = KeyCode::None;
		//if (!keyCallbacks.contains(key)) return;
		//std::vector<InputCallbackContainer>& vec = keyCallbacks[key][0];
		//int size = vec.size();
		//for (int i = 0; i < size; i++) (vec[i].component->*vec[i].callback)();
	}

	void GetKeyRelease(KeyCode key)
	{
		lastReleasedKey = key;
		lastPressedKey = KeyCode::None;
		//if (!keyCallbacks.contains(key)) return;
		//std::vector< InputCallbackContainer>& vec = keyCallbacks[key][1];
		//int size = vec.size();
		//for (int i = 0; i < size; i++) (vec[i].component->*vec[i].callback)();
	}
};
