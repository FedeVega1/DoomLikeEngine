#include "pch.h"
#include "Game.h"
#include "GameObjects.h"
#include "VectorMath.h"
#include "CameraComponent.h"
#include "World.h"
#include "Renderer.h"
#include <algorithm>
#include "SDL3/SDL.h"
#include "SDL3/SDL_video.h"
#include "SDLRenderer.h"

auto surfaceDeleter = [](SDL_Surface* surface) { SDL_DestroySurface(surface); };
using SurfacePtr = std::unique_ptr<SDL_Surface, decltype(surfaceDeleter)>;

long SDLRenderer::InitRenderer_SDL(const SDL_WindowID& windID)
{
	SDL_Window* window = SDL_GetWindowFromID(windID);
	renderer = SDL_CreateRenderer(window, nullptr);

	screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT);
	
	if (!SDL_SetTextureScaleMode(screenTexture, SDL_SCALEMODE_NEAREST))
	{
		OLOG_EF("Couldn't set the screen texture \"scale mode\" to nearest! - {0}", SDL_GetError());
		return 0L;
	}

	screenBuffer = new unsigned long[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT] {0};
	drawBuffer = new unsigned long[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT] {0};

	walls = std::vector<ProcessedWall>();
	loadedTexturesMap = std::map<std::wstring, BaseTexture>();

	LoadTexture(L"test Wall.bmp");
	LoadTexture(L"test Ceilling.bmp", 2);
	LoadTexture(L"test Floor.bmp", 2);
	return 0L;
}

void SDLRenderer::RenderScreen()
{
	if (!SDL_UpdateTexture(screenTexture, nullptr, screenBuffer, DEFAULT_BUFFER_WIDTH * sizeof(unsigned int)))
	{
		OLOG_EF("Failed to Blit main screen texture! - {0}", SDL_GetError());
		return;
	}

	if (!SDL_RenderClear(renderer))
	{
		OLOG_EF("Falied to clear the back buffer! - {0}", SDL_GetError());
		return;
	}
	
	if (!SDL_RenderTexture(renderer, screenTexture, nullptr, nullptr))
	{
		OLOG_EF("Failed to render the screen texture into the back buffer! - {0}", SDL_GetError());
		return;
	}

	if (!SDL_RenderPresent(renderer))
	{
		OLOG_EF("Failed to render the back buffer into the screen! - {0}", SDL_GetError());
		return;
	}

	if (debugStepDraw)
	{
		Sleep(1);
		ToggleStepDraw(false);
	}
}

void SDLRenderer::DebugUpdateScreen() const 
{
	if (!SDL_UpdateTexture(screenTexture, nullptr, drawBuffer, DEFAULT_BUFFER_WIDTH * sizeof(unsigned int)))
	{
		OLOG_EF("Failed to Blit main screen texture! - {0}", SDL_GetError());
		return;
	}

	if (!SDL_RenderClear(renderer))
	{
		OLOG_EF("Falied to clear the back buffer! - {0}", SDL_GetError());
		return;
	}

	if (!SDL_RenderTexture(renderer, screenTexture, nullptr, nullptr))
	{
		OLOG_EF("Failed to render the screen texture into the back buffer! - {0}", SDL_GetError());
		return;
	}

	if (!SDL_RenderPresent(renderer))
	{
		OLOG_EF("Failed to render the back buffer into the screen! - {0}", SDL_GetError());
		return;
	}
}

void SDLRenderer::PaintScreen(const Color& color)
{
	for (int y = 0; y < DEFAULT_BUFFER_HEIGHT; y++)
	{
		for (int x = 0; x < DEFAULT_BUFFER_WIDTH; x++)
			drawBuffer[PixelPos(x, y)] = color.ToDWORD(true);
	}
}

void SDLRenderer::DrawPixel(int x, int y, const Color& color)
{
	x = std::clamp(x, 0, DEFAULT_BUFFER_WIDTH - 1);
	y = std::clamp(y, 0, DEFAULT_BUFFER_HEIGHT - 1);

	drawBuffer[PixelPos(x, y)] = color.ToDWORD(true);

	if (debugStepDraw) DebugUpdateScreen();
}

void SDLRenderer::LoadTexture(const std::wstring& texName, const float& tilling)
{
	const std::string parsedTexName = WStringToString(texName);
	SurfacePtr surface(SDL_LoadBMP(parsedTexName.c_str()));

	if (!surface)
	{
		OLOG_EF("Failed to load texture {0}! - {1}", parsedTexName, SDL_GetError());
		return;
	}

	SurfacePtr convertedSurface(SDL_ConvertSurface(surface.get(), SDL_PIXELFORMAT_ARGB8888));
	surface.reset();

	if (!convertedSurface)
	{
		OLOG_EF("Couldn't convert the loaded texture {0} to the correct pixel format! - {1}", parsedTexName, SDL_GetError());
		return;
	}

	loadedTexturesMap.insert(std::pair(texName, BaseTexture
	{
		texName,
		convertedSurface->w,
		convertedSurface->h,
		tilling,
		new unsigned long[convertedSurface->w * convertedSurface->h]
	}));

	if (!SDL_LockSurface(convertedSurface.get()))
	{
		OLOG_EF("Failed to lock converted surface for texture {0}! - {1}", parsedTexName, SDL_GetError());
		return;
	}

	unsigned long*& textureBuffer = loadedTexturesMap[texName].textureBuffer;
	for (int y = 0; y < convertedSurface->h; y++)
	{
		unsigned int* rowSrc = reinterpret_cast<unsigned int*>(static_cast<unsigned char*>(convertedSurface->pixels) + y * convertedSurface->pitch);
		std::copy(rowSrc, rowSrc + convertedSurface->w, textureBuffer + (y * convertedSurface->w));
	}

	SDL_UnlockSurface(convertedSurface.get());
	OLOG_L("Loaded Texture: {0}", parsedTexName);
}

void SDLRenderer::GetTextureMap(const std::wstring& texName, BaseTexture& texture)
{
	if (!loadedTexturesMap.contains(texName)) return;
	texture = loadedTexturesMap[texName];
}

SDLRenderer::SDLRenderer() : renderer(nullptr), screenTexture(nullptr)
{

}

SDLRenderer::~SDLRenderer() { Dispose(); }

void SDLRenderer::Dispose()
{
	if (screenTexture)
	{
		SDL_DestroyTexture(screenTexture);
		screenTexture = nullptr;
	}

	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	for (std::pair<std::wstring, BaseTexture> iterator : loadedTexturesMap)
		delete[] iterator.second.textureBuffer;

	loadedTexturesMap.clear();

	delete[] screenBuffer;
	screenBuffer = nullptr;

	delete[] drawBuffer;
	drawBuffer = nullptr;
}