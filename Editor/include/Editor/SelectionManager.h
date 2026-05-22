#pragma once

namespace Editor
{
    struct DrawingData
    {
        Core::Color wallTopColor, wallInnerColor, wallBtmColor;
        Core::Color sectorCeilingColor, sectorFloorColor;
        float sectorCeilingHeight = 0.f, sectorFloorHeight = 0.f;
    };

    class SelectionManager
    {
    public:
        explicit SelectionManager(MapData& mapData, float defCeilingHeight, float defFloorHeight);

        void Select(GUID id, SelectableType type);
        void Deselect(GUID id, SelectableType type);
        void Toggle(GUID id, SelectableType type);
        void Clear();

        void ToggleDrawingMode(bool toggle) { onDrawingMode = toggle; }

        bool IsOnDrawingMode() const { return onDrawingMode; }
        bool IsSelected(GUID id, SelectableType type) const;
        bool HasSelection() const;
        const std::vector<SelectedItem>& GetSelected() const;
        std::vector<SelectedItem> GetSelectedOfType(SelectableType type) const;

        std::optional<GUID> GetFirstOfType(SelectableType type) const;

        MapData& GetMapData() const { return mapData; }
        DrawingData& GetDrawingData() { return currentDrawingData; }

    private:
        DrawingData currentDrawingData;
        MapData& mapData;
        std::vector<SelectedItem> selectedItems;

        bool onDrawingMode;
    };
}
