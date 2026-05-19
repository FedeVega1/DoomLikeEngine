#pragma once

struct GridTheme
{
	ImVec4 gridOriginHorizontal, gridOriginVertical;
	ImVec4 gridOddVerticalLine, gridOddHorizontalLine;
	ImVec4 gridEvenVerticalLine, gridEvenHorizontalLine;
	ImVec4 gridBackgrund;

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
