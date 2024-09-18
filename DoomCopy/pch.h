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

static SinCosTable SCTABLE;
