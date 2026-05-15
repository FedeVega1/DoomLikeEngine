#pragma once

struct EditorMapping
{
	ImGuiKey key, modifier;

	bool HasModifierKey() const { return modifier != ImGuiMod_None; }

	bool IsKeyPressed(bool repeating) const
	{
		if (HasModifierKey() && !ImGui::IsKeyPressed(modifier, true)) return false;
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
	GridHotKeys grid;
};

struct EditorHotkeys
{
	MapEditorHotKeys mapEditor;
};


inline constexpr EditorHotkeys MAIN_DEFAULT_HOTKEYS = EditorHotkeys
{
	MapEditorHotKeys
	{
		GridHotKeys
		{
			EditorMapping { ImGuiKey_KeypadAdd, ImGuiMod_None },
			EditorMapping { ImGuiKey_KeypadSubtract, ImGuiMod_None },

			EditorAxisMapping
			{
				EditorMapping { ImGuiKey_KeypadAdd, ImGuiMod_Ctrl },
				EditorMapping { ImGuiKey_KeypadSubtract, ImGuiMod_Ctrl }
			},
			EditorAxisMapping
			{
				EditorMapping { ImGuiKey_LeftArrow, ImGuiMod_None },
				EditorMapping { ImGuiKey_RightArrow, ImGuiMod_None }
			},
			EditorAxisMapping
			{
				EditorMapping { ImGuiKey_UpArrow, ImGuiMod_None },
				EditorMapping { ImGuiKey_DownArrow, ImGuiMod_None }
			}
		}
	}
};
