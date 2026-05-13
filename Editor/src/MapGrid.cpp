#include "pch.h"

#include <imgui.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include "Core/VectorMath.h"
#include "Editor/EditorTheme.h"
#include "Editor/EditorHotKeys.h"
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

		if (direction)
		{
			if ((GetLineIndex() % 2) == 0) return ImGui::GetColorU32(MAIN_DEFAULT_THEME.gridTheme.gridEvenHorizontalLine);
			return ImGui::GetColorU32(MAIN_DEFAULT_THEME.gridTheme.gridOddHorizontalLine);
		}

		if ((GetLineIndex() % 2) == 0) return ImGui::GetColorU32(MAIN_DEFAULT_THEME.gridTheme.gridEvenVerticalLine);
		return ImGui::GetColorU32(MAIN_DEFAULT_THEME.gridTheme.gridOddVerticalLine);
	}

	int GridLineData::GetLineThickness() const
	{
		if (IsOnOrigin()) return MAIN_DEFAULT_THEME.gridTheme.gridOriginThickness;
		if ((GetLineIndex() % 2) == 0) return MAIN_DEFAULT_THEME.gridTheme.gridEvenLineThickness;
		return MAIN_DEFAULT_THEME.gridTheme.gridOddLineThickness;
	}
	
	void MapGrid::HandleInputsNoFocus()
	{
		HandleHotKeys();
	}

	void MapGrid::HandleInputs()
	{
		HandleZoom();
		HandlePanning();
	}

	void MapGrid::HandleZoom()
	{
		float mWheelScroll = ImGui::GetIO().MouseWheel;
		if (std::abs(mWheelScroll) > Core::K_EPSILON) ZoomGrid(mWheelScroll);
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
		MoveGrid(originAtPanStart + (currentPos - panStartPos));
	}

	void MapGrid::HandleHotKeys()
	{
		if (MAIN_DEFAULT_HOTKEYS.mapEditor.grid.increaseGridSize.IsKeyPressed(false)) UpdateGridSize(gridSize * 2);
		if (MAIN_DEFAULT_HOTKEYS.mapEditor.grid.decreaseGridSize.IsKeyPressed(false)) UpdateGridSize(gridSize / 2);

		ZoomGrid(MAIN_DEFAULT_HOTKEYS.mapEditor.grid.zoomAxis.GetAxis(false));

		float x = MAIN_DEFAULT_HOTKEYS.mapEditor.grid.panHorizontal.GetAxis(true);
		float y = MAIN_DEFAULT_HOTKEYS.mapEditor.grid.panVertical.GetAxis(true);
		MoveGrid(origin - Core::Vector2(x, y) * hotKeyPanSpeed);
	}

	void MapGrid::ZoomGrid(float ammount) { zoom = std::clamp(zoom - (ammount * .1f), .1f, 2.f); }

	void MapGrid::MoveGrid(Core::Vector2 ammount)
	{
		origin = ammount;
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

		float cellSize = GetCellSize();

		float firstLineX = roundedOrigin.x - FloorPointPosition(origin.x - mainWindowData.pos.x);
		for (float x = firstLineX; x < mainWindowData.pos.x + mainWindowData.size.x; x += cellSize)
			DrawGridLine(drawList, GridLineData { roundedOrigin, Core::Vector2(x, mainWindowData.pos.y), false, cellSize });

		float firstLineY = roundedOrigin.y - FloorPointPosition(origin.y - mainWindowData.pos.y);
		for (float y = firstLineY; y < mainWindowData.pos.y + mainWindowData.size.y; y += cellSize)
			DrawGridLine(drawList, GridLineData{ roundedOrigin, Core::Vector2(mainWindowData.pos.x, y), true, cellSize });
	}

	void MapGrid::DrawGridLine(ImDrawList* const drawList, const GridLineData& lineData) const
	{
		drawList->AddLine(lineData.position, lineData.GetLineEndPoint(mainWindowData.size), lineData.GetLineColor(), 
			lineData.GetLineThickness());
	}
}
