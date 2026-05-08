#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <windows.h>
#include <cstdint>
#include <memory>
#include "OutputLogger.h"
#include <random>
#include <vector>
#include <chrono>
#include <numbers>
#include <map>
#include <unordered_map>
#include <array>
#include <functional>

#include <Core/VectorMath.h>
#include <Core/WorldTypes.h>
#include <Core/BSPFileIO.h>

using namespace Core;

template<typename T>
struct SortData
{
	T* arr;
	std::function<int(T)> getValue;
	bool inverse;
};

template<typename T>
int Partition(SortData<T>& sortData, int left, int right)
{
	int pivot = sortData.getValue(sortData.arr[left]);
	while (true)
	{
		while ((sortData.inverse && sortData.getValue(sortData.arr[left]) > pivot) ||
			(!sortData.inverse && sortData.getValue(sortData.arr[left]) < pivot)) left++;

		while ((sortData.inverse && sortData.getValue(sortData.arr[right]) < pivot) ||
			(!sortData.inverse && sortData.getValue(sortData.arr[right]) > pivot)) right--;

		if (left < right)
		{
			if (sortData.getValue(sortData.arr[left]) == sortData.getValue(sortData.arr[right])) return right;

			T temp = sortData.arr[left];
			sortData.arr[left] = sortData.arr[right];
			sortData.arr[right] = temp;
			continue;
		}

		return right;
	}
}

template<typename T>
void Quick_Sort(SortData<T>& sortData, int left, int right)
{
	if (left < right)
	{
		int pivot = Partition(sortData, left, right);

		if (pivot > 1) Quick_Sort(sortData, left, pivot - 1);
		if (pivot + 1 < right) Quick_Sort(sortData, pivot + 1, right);
	}
}

std::string WStringToString(const std::wstring& wstr);

enum KeyCode
{
	None = 0,
	Backspace, Tab, Enter, Shift, Ctrl, Alt, Pause,
	CapsLock, Esc, Spacebar, PageUp, PageDown, End, Home,
	Left, Up, Right, Down, Select, Insert, Del,

	Number0, Number1, Number2, Number3, Number4,
	Number5, Number6, Number7, Number8, Number9,

	A, B, C, D, E, F, G, H, I, J, K, L, M,
	N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

	Numpad0, Numpad1, Numpad2, Numpad3, Numpad4,
	Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
	Mult, Add, Sep, Subs, Decimal, Divide,

	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

	LeftMouse, RightMouse, MiddleMouse, X1Mouse, X2Mouse
};
