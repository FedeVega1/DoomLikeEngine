#pragma once

#include "Core/VectorMath.h"

#define IMGUI_ENABLE_FREETYPE
#define ImDrawList_USE_ANTIALIASING 1

#define IM_VEC2_CLASS_EXTRA \
    ImVec2(const Core::Vector2& v) : x(v.x), y(v.y) {} \
    operator Core::Vector2() const { return Core::Vector2(x, y); }
