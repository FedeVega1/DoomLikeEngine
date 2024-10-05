#include "pch.h"
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

void D2Renderer::PaintScreen(Color color)
{
	for (int y = 0; y < DEFAULT_BUFFER_HEIGHT; y++)
	{
		for (int x = 0; x < DEFAULT_BUFFER_WIDTH; x++)
			buffer.get()[PixelPos(x, y)] = color.ToDWORD(false);
	}
}

void D2Renderer::DrawPixel(int x, int y, Color color)
{
	if (x > DEFAULT_BUFFER_WIDTH) x = DEFAULT_BUFFER_WIDTH;
	else if (x < 0) x = 0;

	if (y > DEFAULT_BUFFER_HEIGHT) y = DEFAULT_BUFFER_HEIGHT;
	else if (y < 0) y = 0;

	buffer.get()[PixelPos(x, y)] = color.ToDWORD(false);
}

void D2Renderer::ProcessGame(HWND hwnd, Game* const game)
{
	PaintScreen(Color(0, 0, 0));

	//size_t count = game->GetEntityCount();
	//for (size_t i = 0; i < count; i++)
	//{
	//    GameObject* go = game->GetGameObject(i);
	//    if (!go) continue;
	//    Vector3 pos = go->GetTransform()->GetPos();
	//    DrawPixel((int) std::roundf(pos.x), (int) std::roundf(pos.y), 0xFF0000);
	//}

	const ProcessedSector* sectors = nullptr;
	int numbSectors = game->GetMainCamera()->GetProcessedSectors(&sectors);
	if (!sectors) return;
	for (int i = 0; i < numbSectors; i++) ProcessSector(sectors[i]);

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

void D2Renderer::ProcessSector(const ProcessedSector& sector)
{
	for (int i = 0; i < sector.numberOfWalls; i++)
		DrawWall(sector.sectorWalls[i]);
}

void D2Renderer::DrawWall(const ProcessedWall& wall)
{
	if (wall.leftBtmPoint.y < 1 && wall.rightBtmPoint.y < 1) return;

	ScreenSpaceWall sWall = GetScreenSpaceWall(wall);
	int dyb = sWall.leftBtmPoint.y - sWall.rightBtmPoint.y;
	int dyt = sWall.leftTopPoint.y - sWall.rightTopPoint.y;
	int dx = sWall.leftBtmPoint.x - sWall.rightBtmPoint.x;
	if (dx == 0) dx = 1;
	int xs = sWall.leftBtmPoint.x;

	if (sWall.leftBtmPoint.x < 0) sWall.leftBtmPoint.x = 0;
	if (sWall.rightBtmPoint.x < 0) sWall.rightBtmPoint.x = 0;

	if (sWall.leftBtmPoint.x > DEFAULT_BUFFER_WIDTH) sWall.leftBtmPoint.x = DEFAULT_BUFFER_WIDTH;
	if (sWall.rightBtmPoint.x > DEFAULT_BUFFER_WIDTH) sWall.rightBtmPoint.x = DEFAULT_BUFFER_WIDTH;

	for (int x = xs; x < sWall.rightBtmPoint.x; x++)
	{
		int y1 = dyb * (x - xs + .5f) / dx + sWall.leftBtmPoint.y;
		int y2 = dyt * (x - xs + .5f) / dx + sWall.leftTopPoint.y;

		if (y1 < 0) y1 = 0;
		if (y2 < 0) y2 = 0;

		if (y1 > DEFAULT_BUFFER_HEIGHT) y1 = DEFAULT_BUFFER_HEIGHT;
		if (y2 > DEFAULT_BUFFER_HEIGHT) y2 = DEFAULT_BUFFER_HEIGHT;

		for (int y = y1; y < y2; y++) DrawPixel(x, y, sWall.color);
	}
}

ScreenSpaceWall D2Renderer::GetScreenSpaceWall(const ProcessedWall& wall)
{
	return ScreenSpaceWall
	{
		Vector2Int((int)std::roundf(wall.leftTopPoint.x * 200 / wall.leftTopPoint.y + HALF_WIDTH), (int)std::roundf(wall.leftTopPoint.z * 200 / wall.leftTopPoint.y + HALF_HEIGHT)),
		Vector2Int((int)std::roundf(wall.rightTopPoint.x * 200 / wall.rightTopPoint.y + HALF_WIDTH), (int)std::roundf(wall.rightTopPoint.z * 200 / wall.rightTopPoint.y + HALF_HEIGHT)),
		Vector2Int((int)std::roundf(wall.leftBtmPoint.x * 200 / wall.leftBtmPoint.y + HALF_WIDTH), (int)std::roundf(wall.leftBtmPoint.z * 200 / wall.leftBtmPoint.y + HALF_HEIGHT)),
		Vector2Int((int)std::roundf(wall.rightBtmPoint.x * 200 / wall.rightBtmPoint.y + HALF_WIDTH), (int)std::roundf(wall.rightBtmPoint.z * 200 / wall.rightBtmPoint.y + HALF_HEIGHT)),
		wall.color
	};
}
