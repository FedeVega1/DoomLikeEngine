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

	SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true);
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
		//renderer.ProcessGame(&mainGame);
		//renderer.RenderScreen();
	}
}

bool HandleEvents()
{
	SDL_Event* event = nullptr;
	while (SDL_PollEvent(event))
	{
		if (!event) continue;
		switch (event->type)
		{
			case SDL_EVENT_KEY_DOWN:
				mainGame.CaptureKeyPress(event->key.raw);
				break;

			case SDL_EVENT_KEY_UP:
				mainGame.CaptureKeyRelease(event->key.raw);
				break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				mainGame.CaptureMousePress(event->button.button);
				break;

			case SDL_EVENT_MOUSE_BUTTON_UP:
				mainGame.CaptureMousePress(event->button.button);
				break;

			case SDL_EVENT_MOUSE_MOTION:
				POINTS mousePos;
				mousePos.x = event->motion.x;
				mousePos.y = event->motion.y;
				mainGame.CaptureMouseMovement(&mousePos, false);
				break;

			case SDL_EVENT_QUIT:
				return false;
		}
	}

	return true;
}

void ClearAndExit()
{
	renderer.~SDLRenderer();
	mainGame.~Game();

	SDL_DestroyProperties(props);
	SDL_DestroyWindow(SDL_GetWindowFromID(wndID));

	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	SDL_Quit();
}
