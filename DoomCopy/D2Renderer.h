#pragma once
#include "d2d1.h"
#include "d2d1helper.h"
#include "dwrite.h"
#include "wincodec.h"

#ifndef D2_RENDER
#define D2_RENDER
#endif

template<class Interface>
inline void ReleaseInterface(Interface** interfaceToRelease)
{
	if (!*interfaceToRelease) return;
	(*interfaceToRelease)->Release();
	(*interfaceToRelease) = NULL;
}

class D2Renderer : public Renderer
{
public:
	virtual void RenderScreen(HWND hwnd) override;
	virtual HRESULT InitRenderer(HWND hwnd) override;
	virtual void DrawPixel(int x, int y, DWORD color) override;
	virtual void PaintScreen(DWORD color) override;
	virtual void ProcessGame(HWND hwnd, std::shared_ptr<Game> game) override;

	D2Renderer();
	~D2Renderer();

private:
	std::shared_ptr<DWORD> buffer;

	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1Bitmap* screenBitmap;
};