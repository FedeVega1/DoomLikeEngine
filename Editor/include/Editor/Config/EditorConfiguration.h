#pragma once

#include "EditorHotKeys.h"
#include "EditorTheme.h"

struct GridConfig
{
	int defaultGridSize;
	float defaultZoom;
	bool dottedLines;

	GridHotKeys hotKeys;
	GridTheme gridTheme;
};

struct MapEditorConfig
{

};

struct EditorConfiguration
{
	GridConfig grid;
	MapEditorConfig mapEditor;
};

class ConfigurationManager
{
public:
	ConfigurationManager()
	{
		currentConfiguration = EditorConfiguration
		{
			GridConfig
			{
				16,
				.5f,
				false,
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
				},
				GridTheme
				{
					ImVec4(0.196f, 0.804f, 0.196f, 1.f),
					ImVec4(1.f, 0.f, 0.f, 1.f),
					ImVec4(0.828f, 0.828f, 0.828f, 1.f),
					ImVec4(0.663f, 0.663f, 0.663f, 1.f),
					ImVec4(0.47f, 0.53f, 0.6f, 1.f),
					ImVec4(0.184f, 0.31f, 0.31f, 1.f),
					ImVec4(0.06f, 0.06f, 0.06f, 0.94f),

					2.f, 2.f, 2.f
				},
			},
			MapEditorConfig
			{

			}
		};
	}

	~ConfigurationManager() = default;

	GridConfig& GetGridConfig() { return currentConfiguration.grid; }
	GridHotKeys& GetGridHotKeys() { return currentConfiguration.grid.hotKeys; }
	GridTheme& GetGridTheme() { return currentConfiguration.grid.gridTheme; }

	MapEditorConfig& GetMapEditorConfig() { return currentConfiguration.mapEditor; }

	static ConfigurationManager INS;

private:
	EditorConfiguration currentConfiguration;
};
