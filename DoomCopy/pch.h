#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <memory>
#include "OutputLogger.h"
#include <random>
#include <vector>
#include <chrono>
#include <numbers>
#include <map>
#include <array>

typedef struct _sinCosTable
{
	double cos[360];
	double sin[360];

	_sinCosTable()
	{
		for (int i = 0; i < 360; i++)
		{
			cos[i] = std::cos(i / 180.0 * std::numbers::pi);
			sin[i] = std::sin(i / 180.0 * std::numbers::pi);
		}
	}

} SinCosTable;

struct Color
{
	bool isLittleEndian;
	BYTE r, g, b;

	Color() : r(0), g(0), b(0), isLittleEndian(false)
	{ }

	Color(BYTE r, BYTE g, BYTE b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		isLittleEndian = false;
	}

	Color(DWORD rgb, bool littleEndian)
	{
		isLittleEndian = littleEndian;

		if (littleEndian)
		{
			this->r = (rgb >> 24) & 0xFF;
			this->g = (rgb >> 16) & 0xFF;
			this->b = (rgb >> 8) & 0xFF;
			return;
		}

		this->r = (rgb >> 16) & 0xFF;
		this->g = (rgb >> 8) & 0xFF;
		this->b = rgb & 0xFF;
	}

	bool operator==(Color other) const { return other.r == r && other.g == g && other.b == b && other.isLittleEndian == isLittleEndian; }
	bool operator==(DWORD other) const { return other == ToDWORD(isLittleEndian); }

	DWORD ToDWORD(bool littleEndian) const
	{
		if (littleEndian) return (((DWORD) r) << 16) | (((DWORD) g) << 8) | ((DWORD) b);
		return (((DWORD) b) << 16) | (((DWORD) g) << 8) | ((DWORD) r);
	}

	std::string ToString() const { return "(R = " + std::to_string(r) + ", G = " + std::to_string(g) + ", B = " + std::to_string(b) + ")"; }
};

static SinCosTable SCTABLE;

static const Color COLOR_BLACK = Color(0, 0, 0);
static const Color COLOR_WHITE = Color(0xFF, 0xFF, 0xFF);
static const Color COLOR_RED = Color(0xFF, 0, 0);
static const Color COLOR_DARKRED = Color(0xAA, 0, 0);
static const Color COLOR_GREEN = Color(0, 0xFF, 0);
static const Color COLOR_DARKGREEN = Color(0, 0xAA, 0);
static const Color COLOR_BLUE = Color(0, 0, 0xFF);
static const Color COLOR_DARKBLUE = Color(0, 0, 0xAA);
