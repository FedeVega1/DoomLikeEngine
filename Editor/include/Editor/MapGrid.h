#pragma once

namespace Editor::Grid
{
	struct WindowData
	{
		Core::Vector2 pos;
		Core::Vector2 size;

		inline Core::Vector2 GetScreenSpaceWindowSize() const { return pos + size; }
	};

	struct GridLineData
	{
		Core::Vector2 roundedOrigin, position;
		bool direction;
		float cellSize;

		bool IsOnOrigin() const { return std::abs(position.x - roundedOrigin.x) < Core::K_EPSILON ||
			std::abs(position.y - roundedOrigin.y) < Core::K_EPSILON; }

		int GetLineIndex() const
		{
			float offset = direction ? (position.y - roundedOrigin.y) : (position.x - roundedOrigin.x);
			return std::abs(Core::FloatToInt(std::round(offset / cellSize)));
		}

		ImU32 GetLineColor() const;
		int GetLineThickness() const;

		Core::Vector2 GetLineEndPoint(const Core::Vector2& windowSize) const 
		{ 
			if (direction) return Core::Vector2(position.x + windowSize.x, position.y);
			return Core::Vector2(position.x, position.y + windowSize.y);
		}
	};

	class MapGrid
	{
	public:
		MapGrid(Core::Vector2 mapMaxSize) : origin(), zoom(1.f), gridSize(8), cellSizeLimit(8.f, 256.f), hotKeyPanSpeed(15.f),
			worldMapMaxSize(mapMaxSize) {}

		~MapGrid() {  }

		void InitializeGrid();

		void Render();

		void HandleInputsNoFocus();
		void HandleInputs();

		float GetCurrentZoom() const { return zoom; }
		int GetCurrentGridSize() const { return gridSize; }
		Core::Vector2 GetCurrentOriginPos() const { return origin; }

	private:
		void HandlePanning();
		void HandleHotKeys();
		void HandleZoom();

		void DrawGridLine(ImDrawList* const drawList, const GridLineData& lineData) const;

		inline float FloorPointPosition(const float& value) const { return std::floor(value / GetCellSize()) * GetCellSize(); }
		inline float GetCellSize() const { return std::clamp((gridSize * cellSizeLimit.x) / zoom, cellSizeLimit.x, cellSizeLimit.y); }

		inline void UpdateGridSize(const int& newValue) { gridSize = std::clamp(newValue, 1, 32); }

		void ClampOrigin();
		void ZoomGrid(float ammount);
		void MoveGrid(Core::Vector2 ammount);

		float zoom, hotKeyPanSpeed;
		int gridSize;
		Core::Vector2 origin, panStartPos, originAtPanStart, cellSizeLimit, worldMapMaxSize;

		WindowData mainWindowData;
	};
}