#pragma once

struct GridTheme
{
	ImVec4 gridOriginHorizontal;
	ImVec4 gridOriginVertical;
	ImVec4 gridOddVerticalLine;
	ImVec4 gridOddHorizontalLine;
	ImVec4 gridEvenVerticalLine;
	ImVec4 gridEvenHorizontalLine;

	float gridOriginThickness;
	float gridOddLineThickness;
	float gridEvenLineThickness;
};

struct DrawingLineTheme
{
	ImVec4 drawingVectorLine;
	ImVec4 orphanVectorLine;
	ImVec4 selectedOrphanVectorLine;
	ImVec4 hoverOrphanVectorLine;

	float drawingLineThickness;
	float orphanDrawingLineThickness;
	float selectedDrawingLineThickness;
	float hoverDrawingLineThickness;
};

struct DrawingPortalTheme
{
	ImVec4 portalLine;
	ImVec4 invalidPortalLine;
	ImVec4 selectedPortalLine;
	ImVec4 hoverPortalLine;

	float portalLineThickness;
	float invalidPortalLineThickness;
	float selectedPortalLineThickness;
	float hoverPortalLineThickness;

	ImVec4 portalConnectionLine;
	ImVec4 selectedPortalConnectionLine;
	ImVec4 hoverPortalConnectionLine;

	float portalConnectionLineThickness;
	float selectedPortalConnectionLineThickness;
	float hoverPortalConnectionLineThickness;
};

struct DrawingWallTheme
{
	ImVec4 wallLine;
	ImVec4 invalidWallLine;
	ImVec4 selectedWallLine;
	ImVec4 hoverWallLine;
	ImVec4 wallNormalLine;

	float wallLineThickness;
	float invalidWallLineThickness;
	float selectedWallLineThickness;
	float hoverWallLineThickness;
	float wallNormalLineThickness;
};

struct DrawingSectorTheme
{
	ImVec4 sectorColor;
	ImVec4 invalidSectorColor;
	ImVec4 selectedSectorColor;
	ImVec4 hoverSectorColor;
};

struct DrawingNodeTheme
{
	ImVec4 nodePoint;
	ImVec4 invalidNodePoint;
	ImVec4 selectedNodePoint;
	ImVec4 hoverNodePoint;

	float nodePointThickness;
	float invalidNodePointThickness;
	float selectedNodePointThickness;
	float hoverNodePointThickness;
};

struct EditorTheme
{
	GridTheme gridTheme;
	DrawingLineTheme lineTheme;
	DrawingWallTheme wallTheme;
	DrawingPortalTheme portalTheme;
	DrawingSectorTheme sectorTheme;
	DrawingNodeTheme nodeTheme;
};

inline constexpr EditorTheme MAIN_DEFAULT_THEME = EditorTheme
{
	GridTheme 
	{
		ImVec4(0.19607843f, 0.80392157f, 0.19607843f, 1.f),
		ImVec4(1.f, 0.f, 0.f, 1.f),
		ImVec4(0.82745098f, 0.82745098f, 0.82745098f, 1.f),
		ImVec4(0.6627451f, 0.6627451f, 0.6627451f, 1.f),
		ImVec4(0.47f, 0.53f, 0.6f, 1.f),
		ImVec4(0.18431373f, 0.30980392f, 0.30980392f, 1.f),

		2.f,
		2.f,
		2.f
	},
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

		ImVec4(0.82745098f, 0.82745098f, 0.82745098f, 1.f),
		ImVec4(1.f, 0.76f, 0.4f, 1.f),
		ImVec4(1.f, 0.63921569f, 0.4f, 1.f),

		1.5f,
		2.f,
		1.5f,
	},
	DrawingSectorTheme
	{
		ImVec4(1.f, 1.f, 0.92941176f, 0.467f),
		ImVec4(1.f, 0.f, 0.f, 0.467f),
		ImVec4(0.98823529f, 0.94901961f, 0.85098039f, 0.467f),
		ImVec4(0.98823529f, 0.86666667f, 0.75294118f, 1.f)
	},
	DrawingNodeTheme
	{
		ImVec4(0.98039216f, 0.92156863f, 0.84313725f, 1.f),
		ImVec4(1.f, 0.f, 0.f, 1.f),
		ImVec4(0.67843137f, 0.84705882f, 0.90196078f, 1.f),
		ImVec4(0.f, 0.f, 1.f, 1.f),

		1.f,
		2.f,
		1.f
	},
};
