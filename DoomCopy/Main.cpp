#include "pch.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include "Main.h"

int main(int argc, char* argv[])
{
	if (!InitializeSystems()) return 0;

	if (!SDL_ShowWindow(SDL_GetWindowFromID(wndID)))
	{
		OLOG_CF("There was an error while trying to show the app window: {0}", SDL_GetError());
		return 1;
	}

	renderer.InitRenderer_SDL(wndID);
	MainLoop();
	ClearAndExit();
	return 0;
}

bool InitializeSystems()
{
	InitLogSystem(true, false);

	if (!SDL_SetAppMetadata(appName, appVersion, appID))
	{
		OLOG_CF("There was an error while setting up App Metadata: {0}", SDL_GetError());
		return false;
	}

	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		OLOG_CF("There was an error while Initializing Video SubSytem: {0}", SDL_GetError());
		return false;
	}

	if (!InitWindow())
	{
		OLOG_CF("There was an error while setting up the main app window: {0}", SDL_GetError());
		return false;
	}

	renderer = SDLRenderer();
	mainGame = Game();

	return true;
}

bool InitWindow()
{
	props = SDL_CreateProperties();
	if (!props) return false;

	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, false);
	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);
	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, false);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, DEFAULT_WINDOW_WIDTH);
	SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, DEFAULT_WINDOW_HEIGHT);

	SDL_Window* wnd = SDL_CreateWindowWithProperties(props);
	if (!wnd) return false;

	wndID = SDL_GetWindowID(wnd);
	return true;
}

void MainLoop()
{
	mainGame.InitUpdate();

	while (HandleEvents())
	{
		mainGame.MainUpdate();
		renderer.ProcessGame(&mainGame);
		renderer.RenderScreen();
	}
}

static KeyCode SDLKeyToKeyCode(SDL_Keycode key)
{
	static const std::unordered_map<SDL_Keycode, KeyCode> keyMap =
	{
		{ SDLK_BACKSPACE, KeyCode::Backspace }, { SDLK_TAB, KeyCode::Tab },
		{ SDLK_RETURN, KeyCode::Enter },        { SDLK_LSHIFT, KeyCode::Shift },
		{ SDLK_RSHIFT, KeyCode::Shift },        { SDLK_LCTRL, KeyCode::Ctrl },
		{ SDLK_RCTRL, KeyCode::Ctrl },          { SDLK_LALT, KeyCode::Alt },
		{ SDLK_RALT, KeyCode::Alt },            { SDLK_PAUSE, KeyCode::Pause },
		{ SDLK_CAPSLOCK, KeyCode::CapsLock },   { SDLK_ESCAPE, KeyCode::Esc },
		{ SDLK_SPACE, KeyCode::Spacebar },      { SDLK_PAGEUP, KeyCode::PageUp },
		{ SDLK_PAGEDOWN, KeyCode::PageDown },   { SDLK_END, KeyCode::End },
		{ SDLK_HOME, KeyCode::Home },           { SDLK_LEFT, KeyCode::Left },
		{ SDLK_UP, KeyCode::Up },               { SDLK_RIGHT, KeyCode::Right },
		{ SDLK_DOWN, KeyCode::Down },           { SDLK_INSERT, KeyCode::Insert },
		{ SDLK_DELETE, KeyCode::Del },
		{ SDLK_0, KeyCode::Number0 }, { SDLK_1, KeyCode::Number1 }, { SDLK_2, KeyCode::Number2 },
		{ SDLK_3, KeyCode::Number3 }, { SDLK_4, KeyCode::Number4 }, { SDLK_5, KeyCode::Number5 },
		{ SDLK_6, KeyCode::Number6 }, { SDLK_7, KeyCode::Number7 }, { SDLK_8, KeyCode::Number8 },
		{ SDLK_9, KeyCode::Number9 },
		{ SDLK_A, KeyCode::A }, { SDLK_B, KeyCode::B }, { SDLK_C, KeyCode::C },
		{ SDLK_D, KeyCode::D }, { SDLK_E, KeyCode::E }, { SDLK_F, KeyCode::F },
		{ SDLK_G, KeyCode::G }, { SDLK_H, KeyCode::H }, { SDLK_I, KeyCode::I },
		{ SDLK_J, KeyCode::J }, { SDLK_K, KeyCode::K }, { SDLK_L, KeyCode::L },
		{ SDLK_M, KeyCode::M }, { SDLK_N, KeyCode::N }, { SDLK_O, KeyCode::O },
		{ SDLK_P, KeyCode::P }, { SDLK_Q, KeyCode::Q }, { SDLK_R, KeyCode::R },
		{ SDLK_S, KeyCode::S }, { SDLK_T, KeyCode::T }, { SDLK_U, KeyCode::U },
		{ SDLK_V, KeyCode::V }, { SDLK_W, KeyCode::W }, { SDLK_X, KeyCode::X },
		{ SDLK_Y, KeyCode::Y }, { SDLK_Z, KeyCode::Z },
		{ SDLK_KP_0, KeyCode::Numpad0 }, { SDLK_KP_1, KeyCode::Numpad1 }, { SDLK_KP_2, KeyCode::Numpad2 },
		{ SDLK_KP_3, KeyCode::Numpad3 }, { SDLK_KP_4, KeyCode::Numpad4 }, { SDLK_KP_5, KeyCode::Numpad5 },
		{ SDLK_KP_6, KeyCode::Numpad6 }, { SDLK_KP_7, KeyCode::Numpad7 }, { SDLK_KP_8, KeyCode::Numpad8 },
		{ SDLK_KP_9, KeyCode::Numpad9 }, { SDLK_KP_MULTIPLY, KeyCode::Mult },
		{ SDLK_KP_PLUS, KeyCode::Add },  { SDLK_KP_MINUS, KeyCode::Subs },
		{ SDLK_KP_PERIOD, KeyCode::Decimal }, { SDLK_KP_DIVIDE, KeyCode::Divide },
		{ SDLK_F1, KeyCode::F1 },   { SDLK_F2, KeyCode::F2 },   { SDLK_F3, KeyCode::F3 },
		{ SDLK_F4, KeyCode::F4 },   { SDLK_F5, KeyCode::F5 },   { SDLK_F6, KeyCode::F6 },
		{ SDLK_F7, KeyCode::F7 },   { SDLK_F8, KeyCode::F8 },   { SDLK_F9, KeyCode::F9 },
		{ SDLK_F10, KeyCode::F10 }, { SDLK_F11, KeyCode::F11 }, { SDLK_F12, KeyCode::F12 },
	};

	auto it = keyMap.find(key);
	if (it == keyMap.end()) return KeyCode::None;
	return it->second;
}

bool HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_EVENT_KEY_DOWN:
			{
				KeyCode key = SDLKeyToKeyCode(event.key.key);
				if (key == KeyCode::Esc && mouseCaptured)
				{
					SDL_SetWindowRelativeMouseMode(SDL_GetWindowFromID(wndID), false);
					mouseCaptured = false;
				}

				if (key == KeyCode::End) renderer.ToggleStepDraw(true);

				mainGame.CaptureKeyPress(static_cast<unsigned long long>(key));
				break;
			}

			case SDL_EVENT_KEY_UP:
				mainGame.CaptureKeyRelease(
					static_cast<unsigned long long>(SDLKeyToKeyCode(event.key.key)));
				break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				if (event.button.button == SDL_BUTTON_LEFT && !mouseCaptured)
				{
					SDL_SetWindowRelativeMouseMode(SDL_GetWindowFromID(wndID), true);
					mouseCaptured = true;
				}
				mainGame.CaptureMousePress(event.button.button - 1);
				break;

			case SDL_EVENT_MOUSE_BUTTON_UP:
				mainGame.CaptureMouseRelease(event.button.button - 1);
				break;

			case SDL_EVENT_MOUSE_MOTION:
				mainGame.CaptureMouseMovement(
					static_cast<int>(event.motion.xrel),
					static_cast<int>(event.motion.yrel),
					mouseCaptured);
				break;

			case SDL_EVENT_QUIT:
				return false;
		}
	}

	return true;
}

void ClearAndExit()
{
	renderer.Dispose();

	SDL_DestroyProperties(props);
	SDL_DestroyWindow(SDL_GetWindowFromID(wndID));

	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	SDL_Quit();
}
