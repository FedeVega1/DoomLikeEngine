#include "pch.h"

#include "Editor/EditorCommands.h"
#include "Editor/EditorTypes.h"

namespace Editor
{
	void PlaceLineSegmentCommand::Execute(MapData& map)
	{
		map.ReinsertNode(EditorNode{ rightNodePos, wall.rightNodeID });
		map.ReinsertWall(wall);
	}

	void PlaceLineSegmentCommand::Undo(MapData& map)
	{
		map.RemoveWall(wall.wallID);
		map.RemoveNode(wall.rightNodeID);
	}

	void GeometryDragCommand::Execute(MapData& map)
	{
		for (const auto& delta : deltas) map.GetNode(delta.nodeID).pos = delta.newPos;
		for (const auto& delta : deltas) map.RefreshWallBoundsForNode(delta.nodeID);
	}

	void GeometryDragCommand::Undo(MapData& map)
	{
		for (const auto& delta : deltas) map.GetNode(delta.nodeID).pos = delta.oldPos;
		for (const auto& delta : deltas) map.RefreshWallBoundsForNode(delta.nodeID);
	}

	void DeleteSectorCommand::Execute(MapData& map)
	{
		for (const auto& wall : sectorWalls) map.RemoveWall(wall.wallID);
		map.RemoveSector(sector.sectorID);
	}

	void DeleteSectorCommand::Undo(MapData& map)
	{
		map.ReinsertSector(sector);
		for (const auto& wall : sectorWalls) map.ReinsertWall(wall);
	}

	void SetPortalCommand::Execute(MapData& map)
	{
		EditorWall& wall = map.GetWall(wallID);
		wall.isPortal = newIsPortal;
		wall.isConnection = newIsConnection;
		wall.portalWallTargetID = newPortalWallTargetID;
		wall.portalTargetSectorID = newPortalTargetSectorID;
	}

	void SetPortalCommand::Undo(MapData& map)
	{
		EditorWall& wall = map.GetWall(wallID);
		wall.isPortal = oldIsPortal;
		wall.isConnection = oldIsConnection;
		wall.portalWallTargetID = oldPortalWallTargetID;
		wall.portalTargetSectorID = oldPortalTargetSectorID;
	}

	void CompositeCommand::Execute(MapData& map)
	{
		for (auto& step : steps)
			step->Execute(map);
	}

	void CompositeCommand::Undo(MapData& map)
	{
		for (auto& step : steps)
			step->Undo(map);
	}

	void CommandHistory::Undo(MapData& map)
	{
		if (undoStack.empty()) return;
		undoStack.back()->Undo(map);
		redoStack.push_back(std::move(undoStack.back()));
		undoStack.pop_back();
	}

	void CommandHistory::Redo(MapData& map)
	{
		if (redoStack.empty()) return;
		redoStack.back()->Execute(map);
		undoStack.push_back(std::move(redoStack.back()));
		redoStack.pop_back();
	}
}
