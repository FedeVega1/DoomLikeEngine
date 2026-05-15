#pragma once

struct GridConfig
{
	int defaultGridSize;
	float defaultZoom;
	bool dottedLines;
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
				false
			},
			MapEditorConfig
			{

			}
		};
	}

	~ConfigurationManager() = default;

	GridConfig& GetGridConfig() { return currentConfiguration.grid; }
	MapEditorConfig& GetMapEditorConfig() { return currentConfiguration.mapEditor; }

	static ConfigurationManager INS;

private:
	EditorConfiguration currentConfiguration;
};
