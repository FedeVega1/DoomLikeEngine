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

		bool IsOnOrigin() const { return std::abs(position.x - roundedOrigin.x) < Core::K_EPSILON || 
			std::abs(position.y - roundedOrigin.y) < Core::K_EPSILON; }

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
		MapGrid(Core::Vector2 mapMaxSize) : origin(), zoom(1.f), gridSize(8), cellSizeLimit(8.f, 256.f), 
			worldMapMaxSize(mapMaxSize) {}

		~MapGrid() {  }

		void InitializeGrid();
		void HandlePanning();
		void Update();
		void Render();

	private:
		void DrawGridLine(ImDrawList* const drawList, const GridLineData& lineData) const;

		inline float FloorPointPosition(const float& value) const { return std::floor(value / GetCellSize()) * GetCellSize(); }
		inline float GetCellSize() const { return std::clamp((gridSize * cellSizeLimit.x) / zoom, cellSizeLimit.x, cellSizeLimit.y); }

		void ClampOrigin();

		float zoom;
		int gridSize;
		Core::Vector2 origin, panStartPos, originAtPanStart, cellSizeLimit, worldMapMaxSize;

		WindowData mainWindowData;
	};
}