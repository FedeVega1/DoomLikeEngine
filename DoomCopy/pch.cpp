#include "pch.h"

std::string WStringToString(const std::wstring& wstr)
{
	if (wstr.empty()) return {};

	int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(size - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, result.data(), size, nullptr, nullptr);
	return result;
}
