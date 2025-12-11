#pragma once

#ifndef SDL_RENDER
#define SDL_RENDER
#endif

class SDLRenderer : Renderer
{
public:
	virtual void RenderScreen() override;
	virtual HRESULT InitRenderer(const HWND const hwnd) override;
	virtual void DrawPixel(int x, int y, const Color& color) override;
	virtual void PaintScreen(const Color& color) override;

	SDLRenderer();
	~SDLRenderer();

	void ToggleStepDraw(bool toggle) { debugStepDraw = toggle; }

protected:
	virtual void LoadTexture(const std::wstring& texName, const float& tilling = 1.0f) override;
	virtual void GetTextureMap(const std::wstring& texName, BaseTexture& texture) override;
};