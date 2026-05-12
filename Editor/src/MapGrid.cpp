#include "pch.h"

#include <imgui.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include "Core/VectorMath.h"
#include "Editor/EditorTheme.h"
#include "Editor/MapGrid.h"

namespace Editor::Grid
{
	ImU32 GridLineData::GetLineColor() const
	{
		if (IsOnOrigin())
		{
			if (direction) return ImGui::GetColorU32(MAIN_DEFAULT_THEME.gridTheme.gridOriginHorizontal);
			return ImGui::GetColorU32(MAIN_DEFAULT_THEME.gridTheme.gridOriginVertical);
		}

		if (direction) return ImGui::GetColorU32(MAIN_DEFAULT_THEME.gridTheme.gridEvenHorizontalLine);
		return ImGui::GetColorU32(MAIN_DEFAULT_THEME.gridTheme.gridEvenVerticalLine);
	}

	int GridLineData::GetLineThickness() const
	{
		if (IsOnOrigin()) return MAIN_DEFAULT_THEME.gridTheme.gridOriginThickness;
		return MAIN_DEFAULT_THEME.gridTheme.gridEvenLineThickness;
	}

	void MapGrid::Update()
	{
		if (!ImGui::IsWindowHovered()) return;
		HandlePanning();
	}

	void MapGrid::InitializeGrid()
	{
		mainWindowData = WindowData{ ImGui::GetCursorScreenPos(), ImGui::GetContentRegionAvail() };
		origin = Core::Vector2(mainWindowData.pos.x + (mainWindowData.size.x / 2), mainWindowData.pos.y + (mainWindowData.size.y / 2));
	}

	void MapGrid::HandlePanning()
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
		{
			panStartPos = ImGui::GetMousePos();
			originAtPanStart = origin;
		}

		if (!ImGui::IsMouseDown(ImGuiMouseButton_Middle)) return;

		Core::Vector2 currentPos = ImGui::GetMousePos();
		origin = originAtPanStart + (currentPos - panStartPos);
		ClampOrigin();
	}

	void MapGrid::ClampOrigin()
	{
		float pixelsPerUnit = GetCellSize() / gridSize;
		Core::Vector2 screenCenter = mainWindowData.pos + mainWindowData.size * .5f;
		Core::Vector2 worldOffset = (origin - screenCenter) / pixelsPerUnit;

		worldOffset.x = std::clamp(worldOffset.x, -worldMapMaxSize.x, worldMapMaxSize.x);
		worldOffset.y = std::clamp(worldOffset.y, -worldMapMaxSize.y, worldMapMaxSize.y);

		origin = screenCenter + worldOffset * pixelsPerUnit;
	}

	void MapGrid::Render()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		mainWindowData = WindowData { ImGui::GetCursorScreenPos(), ImGui::GetContentRegionAvail() };

		Core::Vector2 roundedOrigin = Core::Vector2(std::round(origin.x), std::round(origin.y));

		float firstLineX = roundedOrigin.x - FloorPointPosition(origin.x - mainWindowData.pos.x);
		for (float x = firstLineX; x < mainWindowData.pos.x + mainWindowData.size.x; x += GetCellSize())
			DrawGridLine(drawList, GridLineData { roundedOrigin, Core::Vector2(x, mainWindowData.pos.y), false });

		float firstLineY = roundedOrigin.y - FloorPointPosition(origin.y - mainWindowData.pos.y);
		for (float y = firstLineY; y < mainWindowData.pos.y + mainWindowData.size.y; y += GetCellSize())
			DrawGridLine(drawList, GridLineData{ roundedOrigin, Core::Vector2(mainWindowData.pos.x, y), true });
	}

	void MapGrid::DrawGridLine(ImDrawList* const drawList, const GridLineData& lineData) const
	{
		drawList->AddLine(lineData.position, lineData.GetLineEndPoint(mainWindowData.size), lineData.GetLineColor(), 
			lineData.GetLineThickness());
	}
}
