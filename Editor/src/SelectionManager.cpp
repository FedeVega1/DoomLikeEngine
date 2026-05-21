#include "pch.h"
#include "Editor/EditorTypes.h"
#include "Editor/SelectionManager.h"

namespace Editor
{
    SelectionManager::SelectionManager(MapData& mapData, float defCeilingHeight, float defFloorHeight) : mapData(mapData), 
        onDrawingMode(false) 
    { 
        currentDrawingData.sectorCeilingHeight = defCeilingHeight;
        currentDrawingData.sectorFloorHeight = defFloorHeight;
    }

    void SelectionManager::Select(GUID id, SelectableType type)
    {
        if (!IsSelected(id, type))
            selectedItems.push_back({ id, type });
    }

    void SelectionManager::Deselect(GUID id, SelectableType type)
    {
        std::erase_if(selectedItems, [id, type](const SelectedItem& item)
        {
            return item.id == id && item.type == type;
        });
    }

    void SelectionManager::Toggle(GUID id, SelectableType type)
    {
        if (IsSelected(id, type)) Deselect(id, type);
        else Select(id, type);
    }

    void SelectionManager::Clear() { selectedItems.clear(); }

    bool SelectionManager::IsSelected(GUID id, SelectableType type) const
    {
        for (const SelectedItem& item : selectedItems) 
            if (item.id == id && item.type == type) return true;
        return false;
    }

    bool SelectionManager::HasSelection() const { return !selectedItems.empty(); }

    const std::vector<SelectedItem>& SelectionManager::GetSelected() const { return selectedItems; }

    std::vector<SelectedItem> SelectionManager::GetSelectedOfType(SelectableType type) const
    {
        std::vector<SelectedItem> result;
        for (const SelectedItem& item : selectedItems)
            if (item.type == type) result.push_back(item);
        return result;
    }

    std::optional<GUID> SelectionManager::GetFirstOfType(SelectableType type) const
    {
        for (const auto& item : selectedItems)
            if (item.type == type) return item.id;
        return std::nullopt;
    }
}
