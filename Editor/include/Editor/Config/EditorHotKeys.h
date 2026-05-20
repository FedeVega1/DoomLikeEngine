#pragma once

struct EditorMapping
{
	ImGuiKey key, modifier;

	bool HasModifierKey() const { return modifier != ImGuiMod_None; }

	bool IsKeyPressed(bool repeating) const
	{
		if (HasModifierKey()) return ImGui::IsKeyChordPressed(modifier | key);
		return ImGui::IsKeyPressed(key, repeating);
	}
};

struct EditorAxisMapping
{
	EditorMapping negativeValue, positiveValue;

	int GetAxis(bool repeating) const
	{
		if (negativeValue.IsKeyPressed(repeating)) return -1;
		if (positiveValue.IsKeyPressed(repeating)) return 1;
		return 0;
	}
};

struct GridHotKeys
{
	EditorMapping increaseGridSize, decreaseGridSize;
	EditorAxisMapping zoomAxis;
	EditorAxisMapping panHorizontal, panVertical;
};

struct MapEditorHotKeys
{
	EditorMapping cancelAction;
};

struct EditorHotKeys
{
	EditorMapping undo, redo;
	EditorMapping newMap, loadMap, saveMap;
};
