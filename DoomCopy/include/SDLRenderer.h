#pragma once

#ifndef SDL_RENDER
#define SDL_RENDER
#endif

class SDLRenderer : public Renderer
{
public:
	virtual void RenderScreen() override;
	virtual long InitRenderer(const HWND const hwnd) override { return 0L; }
	virtual long InitRenderer_SDL(const SDL_WindowID& windID) override;
	virtual void DrawPixel(int x, int y, const Color& color) override;
	virtual void PaintScreen(const Color& color) override;
	virtual void Dispose() override;

	SDLRenderer();
	~SDLRenderer();

	void ToggleStepDraw(bool toggle) { debugStepDraw = toggle; }

protected:
	virtual void LoadTexture(const std::wstring& texName, const float& tilling = 1.0f) override;
	virtual void GetTextureMap(const std::wstring& texName, BaseTexture& texture) override;

private:
	struct SDL_Texture* screenTexture;
	struct SDL_Renderer* renderer;

	void DebugUpdateScreen() const;
};