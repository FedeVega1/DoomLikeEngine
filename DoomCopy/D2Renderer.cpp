#include "framework.h"
#include "Game.h"
#include "Renderer.h"
#include "D2Renderer.h"

HRESULT D2Renderer::InitRenderer(HWND hwnd)
{
	buffer = std::shared_ptr<DWORD>(new DWORD[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT]{0});

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (IS_ERROR(hr))
	{
		OLOG_EF("Error while Creating D2D1 Factory! {0}", hr);
		return hr;
	}

	D2D1_PIXEL_FORMAT pixelFormat = D2D1_PIXEL_FORMAT{ DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE };
	D2D1_RENDER_TARGET_PROPERTIES renderProperties = D2D1_RENDER_TARGET_PROPERTIES
	{
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		pixelFormat,
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	};

	hr = factory->CreateHwndRenderTarget(renderProperties, D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)), &renderTarget);
	if (IS_ERROR(hr))
	{
		OLOG_EF("Error while creating Render Target! {0}", hr);
		return hr;
	}

	hr = renderTarget->CreateBitmap(D2D1::SizeU(DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT), D2D1::BitmapProperties(pixelFormat), &screenBitmap);

	if (IS_ERROR(hr))
		OLOG_EF("Error while creating Bitmap! {0}", hr);

	return hr;
}

void D2Renderer::RenderScreen(HWND hwnd)
{
	if (!renderTarget || !screenBitmap) return;
	renderTarget->BeginDraw();
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	RECT rc;
	GetClientRect(hwnd, &rc);
	D2D1_RECT_U rect = D2D1::RectU(rc.left, rc.top, rc.right, rc.bottom);
	HRESULT hr = screenBitmap->CopyFromMemory(&rect, buffer.get(), DEFAULT_BUFFER_WIDTH * sizeof(DWORD));

	renderTarget->DrawBitmap(screenBitmap);
	hr = renderTarget->EndDraw();

	if (IS_ERROR(hr)) OLOG_EF("There was an error while rendering! {0}", hr);
}

void D2Renderer::PaintScreen(DWORD color)
{
	for (int y = 0; y < DEFAULT_BUFFER_HEIGHT; y++)
	{
		for (int x = 0; x < DEFAULT_BUFFER_WIDTH; x++)
			buffer.get()[PixelPos(x, y)] = color;
	}
}

void D2Renderer::DrawPixel(int x, int y, DWORD color)
{
	if (x > DEFAULT_BUFFER_WIDTH) x = DEFAULT_BUFFER_WIDTH;
	else if (x < 0) x = 0;

	if (y > DEFAULT_BUFFER_HEIGHT) y = DEFAULT_BUFFER_HEIGHT;
	else if (y < 0) y = 0;

	buffer.get()[PixelPos(x, y)] = color;
}

void D2Renderer::ProcessGame(HWND hwnd, std::shared_ptr<Game> game)
{
	PaintScreen(0);

	//for (int i = 0; i < 250; i++)
	//	DrawPixel(55 + i, game->GetPixel(), 0x000000FF);

	InvalidateRect(hwnd, NULL, false);
}

D2Renderer::D2Renderer() : factory(NULL), renderTarget(NULL), screenBitmap(NULL)
{ }

D2Renderer::~D2Renderer()
{
	ReleaseInterface(&factory);
	ReleaseInterface(&renderTarget);
	ReleaseInterface(&screenBitmap);
}
