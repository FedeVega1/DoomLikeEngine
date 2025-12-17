#include "pch.h"
#include "Game.h"
#include "GameObjects.h"
#include "VectorMath.h"
#include "CameraComponent.h"
#include "World.h"
#include "Renderer.h"
#include "SDL3/SDL_video.h"
#include "SDLRenderer.h"

HRESULT SDLRenderer::InitRenderer(const HWND const _hwnd)
{
	return S_OK;
}

void SDLRenderer::RenderScreen()
{

}

void SDLRenderer::PaintScreen(const Color& color)
{

}

void SDLRenderer::DrawPixel(int x, int y, const Color& color)
{

}

void SDLRenderer::LoadTexture(const std::wstring& texName, const float& tilling)
{

}

void SDLRenderer::GetTextureMap(const std::wstring& texName, BaseTexture& texture)
{

}

SDLRenderer::SDLRenderer()
{

}

SDLRenderer::~SDLRenderer()
{

}