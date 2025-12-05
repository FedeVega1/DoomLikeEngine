#include "pch.h"
#include "Game.h"
#include "GameObjects.h"
#include "VectorMath.h"
#include "CameraComponent.h"
#include "World.h"
#include "Renderer.h"
#include "GDIRenderer.h"

HRESULT GDIRenderer::InitRenderer(HWND _hwnd)
{
    screenBuffer = new DWORD[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT] {0};
    drawBuffer = new DWORD[DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT] {0};

    walls = std::vector<ProcessedWall>();
    hwnd = _hwnd;

    HDC hDesktop = GetDC(hwnd);
    memHDC = CreateCompatibleDC(hDesktop);
    hbmp = CreateCompatibleBitmap(hDesktop, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT);

    loadedTexturesMap = std::map<std::wstring, BaseTexture>();

    bitmapInfo = BITMAPINFO { 
        BITMAPINFOHEADER 
        { 
            sizeof(BITMAPINFOHEADER),
            DEFAULT_BUFFER_WIDTH,
            DEFAULT_BUFFER_HEIGHT,
            1,
            32,
            BI_RGB,
            DEFAULT_BUFFER_WIDTH * DEFAULT_BUFFER_HEIGHT
        } 
    };

    SelectObject(memHDC, hbmp);

    LoadTexture(L"test Wall.bmp");
    LoadTexture(L"test Ceilling.bmp", 2);
    LoadTexture(L"test Floor.bmp", 2);
    return S_OK;
}

void GDIRenderer::RenderScreen()
{
    if (bitmapInfo.bmiHeader.biSize == 0) return;

    HDC hdc = GetDC(hwnd);

    int scanlines = StretchDIBits(hdc, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT, screenBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    if (scanlines == 0) OLOG_E("Failed to Draw Bitmap!");

    if (debugStepDraw)
    {
        Sleep(1);
        ToggleStepDraw(false);
    }

    ReleaseDC(hwnd, hdc);
}

void GDIRenderer::PaintScreen(Color color)
{
    for (int y = 0; y < DEFAULT_BUFFER_HEIGHT; y++)
    {
        for (int x = 0; x < DEFAULT_BUFFER_WIDTH; x++)
            drawBuffer[PixelPos(x, y)] = color.ToDWORD(true);
    }
}

void GDIRenderer::DrawPixel(int x, int y, Color color)
{
    x = std::clamp(x, 0, DEFAULT_BUFFER_WIDTH - 1);
    y = std::clamp(y, 0, DEFAULT_BUFFER_HEIGHT - 1);

    drawBuffer[PixelPos(x, y)] = color.ToDWORD(true);

    if (debugStepDraw)
    {
        HDC hdc = GetDC(hwnd);
        int scanlines = StretchDIBits(hdc, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, 0, DEFAULT_BUFFER_WIDTH, DEFAULT_BUFFER_HEIGHT, drawBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
        if (scanlines == 0) OLOG_E("DEBUG - Failed to Draw Bitmap!");
        //Sleep(1);
        ReleaseDC(hwnd, hdc);
    }
}

void GDIRenderer::LoadTexture(const std::wstring& texName, const float& tilling)
{
    HBITMAP hbmp = (HBITMAP)LoadImage(nullptr, texName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hbmp)
    {
        OLOG_E("Failed to load Texture: {0}. Texture source was not found!");
        return;
    }

    BITMAP bmp;
    GetObject(hbmp, sizeof(BITMAP), &bmp);

    loadedTexturesMap.insert(std::pair(texName, BaseTexture
    {
        texName,
        bmp.bmWidth,
        bmp.bmHeight,
        tilling,
        new DWORD[bmp.bmWidth * bmp.bmHeight]
    }));

    BITMAPINFO bmpInfo = BITMAPINFO
    {
        sizeof(BITMAPINFOHEADER),
        bmp.bmWidth,
        -bmp.bmHeight,
        1,
        bmp.bmBitsPixel,
        BI_RGB
    };

    if (!GetDIBits(memHDC, hbmp, 0, bmp.bmHeight, loadedTexturesMap[texName].textureBuffer, &bmpInfo, DIB_RGB_COLORS))
        OLOG_E("Falied to load texture: {0}");

    OLOG_L("Loaded texture: {0}");
    DeleteObject(hbmp);
}

void GDIRenderer::GetTextureMap(const std::wstring& texName, BaseTexture& texture)
{
    if (!loadedTexturesMap.contains(texName)) return;
    texture = loadedTexturesMap[texName];
}

GDIRenderer::GDIRenderer() : bitmapInfo(), hbmp(nullptr), memHDC(nullptr)
{ }

GDIRenderer::~GDIRenderer()
{
    DeleteObject(hbmp);

    for (std::pair<std::wstring, BaseTexture> iterator : loadedTexturesMap)
        delete[] iterator.second.textureBuffer;

    delete[] screenBuffer;
    delete[] drawBuffer;
}
