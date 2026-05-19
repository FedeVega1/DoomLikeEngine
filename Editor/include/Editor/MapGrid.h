#pragma once

namespace Editor::Grid
{
	inline constexpr float MAX_ZOOM = 2.f;
	inline constexpr float MIN_ZOOM = .1f;

	inline constexpr int MIN_GRID_SIZE = 1;
	inline constexpr int MAX_GRID_SIZE = 64;

	inline constexpr int GRID_ZOOM_STEP = 8;

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

		bool IsOnOrigin() const { return GetLineIndex() == 0; }

		int GetLineIndex() const
		{
			float offset = direction ? (position.y - roundedOrigin.y) : (position.x - roundedOrigin.x);
			return std::abs(Core::FloatToInt(std::round(offset / cellSize)));
		}

		ImU32 GetLineColor() const;
		float GetLineThickness() const;

		Core::Vector2 GetLineEndPoint(const Core::Vector2& windowSize) const 
		{ 
			if (direction) return Core::Vector2(position.x + windowSize.x, position.y);
			return Core::Vector2(position.x, position.y + windowSize.y);
		}
	};

	class MapGrid
	{
	public:
		MapGrid(Core::Vector2 mapMaxSize);
		~MapGrid() = default;

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
		void DrawDottedGrid(ImDrawList* const drawList, const Core::Vector2& roundedOrigin, float cellSize) const;

		inline float FloorPointPosition(const float& value) const { return std::floor(value / GetCellSize()) * GetCellSize(); }
		inline float GetCellSize() const { return std::clamp((gridSize * GRID_ZOOM_STEP) / zoom, cellSizeLimit.x, cellSizeLimit.y); }

		inline void UpdateGridSize(const int& newValue) { gridSize = std::clamp(newValue, MIN_GRID_SIZE, MAX_GRID_SIZE); }

		void ClampOrigin();
		void ZoomGrid(float ammount);
		void MoveGrid(Core::Vector2 ammount);

		float zoom, hotKeyPanSpeed;
		int gridSize;
		Core::Vector2 origin, panStartPos, originAtPanStart, cellSizeLimit, worldMapMaxSize;

		WindowData mainWindowData;
	};
}