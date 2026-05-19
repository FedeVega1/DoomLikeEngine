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
	DrawingLineTheme lineTheme;
	DrawingNodeTheme nodeTheme;
	DrawingWallTheme wallTheme;
	DrawingPortalTheme portalTheme;
	DrawingSectorTheme sectorTheme;
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
				DrawingLineTheme
				{
					ImVec4(1.f, 1.f, 0.f, .5f),
					ImVec4(8.f, 0.f, 0.f, .5f),
					ImVec4(1.f, 0.647f, 0.f, .5f),
					ImVec4(1.f, 0.27f, 0.f, .5f),

					1.f,
					1.f,
					2.f,
					1.f
				},
				DrawingNodeTheme
				{
					ImVec4(0.98f, 0.922f, 0.843f, 1.f),
					ImVec4(1.f, 0.f, 0.f, 1.f),
					ImVec4(0.678f, 0.847f, 0.902f, 1.f),
					ImVec4(0.f, 0.f, 1.f, 1.f),

					1.f,
					2.f,
					1.f
				},
				DrawingWallTheme
				{
					ImVec4(1.f, 1.f, 0.f, 1.f),
					ImVec4(8.f, 0.f, 0.f, 1.f),
					ImVec4(1.f, 0.647f, 0.f, 1.f),
					ImVec4(1.f, 0.27f, 0.f, 1.f),
					ImVec4(1.f, 8.f, 0.f, 1.f),

					2.f,
					2.f,
					3.f,
					2.f
				},
				DrawingPortalTheme
				{
					ImVec4(1.f, .4f, .8f, 1.f),
					ImVec4(1.f, .4f, 0.f, 1.f),
					ImVec4(1.f, 0.2f, 0.73f, 1.f),
					ImVec4(1.f, 0.f, 0.67f, 1.f),

					2.f,
					2.f,
					3.f,
					2.f,

					ImVec4(0.828f, 0.828f, 0.828f, 1.f),
					ImVec4(1.f, 0.76f, 0.4f, 1.f),
					ImVec4(1.f, 0.639f, 0.4f, 1.f),

					1.5f,
					2.f,
					1.5f,
				},
				DrawingSectorTheme
				{
					ImVec4(1.f, 1.f, 0.929f, 0.467f),
					ImVec4(1.f, 0.f, 0.f, 0.467f),
					ImVec4(0.988f, 0.949f, 0.851f, 0.467f),
					ImVec4(0.988f, 0.87f, 0.753f, 1.f)
				}
			}
		};
	}

	~ConfigurationManager() = default;

	GridConfig& GetGridConfig() { return currentConfiguration.grid; }
	GridHotKeys& GetGridHotKeys() { return currentConfiguration.grid.hotKeys; }
	GridTheme& GetGridTheme() { return currentConfiguration.grid.gridTheme; }

	DrawingLineTheme& GetLineTheme() { return currentConfiguration.mapEditor.lineTheme; }
	DrawingNodeTheme& GetNodeTheme() { return currentConfiguration.mapEditor.nodeTheme; }
	DrawingWallTheme& GetWallTheme() { return currentConfiguration.mapEditor.wallTheme; }
	DrawingPortalTheme& GetPortalTheme() { return currentConfiguration.mapEditor.portalTheme; }
	DrawingSectorTheme& GetSectorTheme() { return currentConfiguration.mapEditor.sectorTheme; }

	MapEditorConfig& GetMapEditorConfig() { return currentConfiguration.mapEditor; }

	static ConfigurationManager INS;

private:
	EditorConfiguration currentConfiguration;
};
