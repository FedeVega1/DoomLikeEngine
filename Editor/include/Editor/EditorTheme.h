#pragma once

struct GridTheme
{
	ImVec4 gridOriginHorizontal, gridOriginVertical;
	ImVec4 gridOddVerticalLine, gridOddHorizontalLine;
	ImVec4 gridEvenVerticalLine, gridEvenHorizontalLine;

	float gridOriginThickness, gridOddLineThickness, gridEvenLineThickness;
};

struct DrawingLineTheme
{
	ImVec4 drawingVectorLine, orphanVectorLine;
	ImVec4 selectedOrphanVectorLine, hoverOrphanVectorLine;

	float drawingLineThickness, orphanDrawingLineThickness;
	float selectedDrawingLineThickness, hoverDrawingLineThickness;
};

struct DrawingPortalTheme
{
	ImVec4 portalLine, invalidPortalLine;
	ImVec4 selectedPortalLine, hoverPortalLine;

	float portalLineThickness, invalidPortalLineThickness;
	float selectedPortalLineThickness, hoverPortalLineThickness;

	ImVec4 portalConnectionLine, selectedPortalConnectionLine, hoverPortalConnectionLine;

	float portalConnectionLineThickness, selectedPortalConnectionLineThickness, hoverPortalConnectionLineThickness;
};

struct DrawingWallTheme
{
	ImVec4 wallLine, invalidWallLine, selectedWallLine;
	ImVec4 hoverWallLine, wallNormalLine;

	float wallLineThickness, invalidWallLineThickness, selectedWallLineThickness;
	float hoverWallLineThickness, wallNormalLineThickness;
};

struct DrawingSectorTheme
{
	ImVec4 sectorColor, invalidSectorColor;
	ImVec4 selectedSectorColor, hoverSectorColor;
};

struct DrawingNodeTheme
{
	ImVec4 nodePoint, invalidNodePoint;
	ImVec4 selectedNodePoint, hoverNodePoint;

	float nodePointThickness, invalidNodePointThickness;
	float selectedNodePointThickness, hoverNodePointThickness;
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
