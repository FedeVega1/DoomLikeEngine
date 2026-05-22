#pragma once

#include "Editor/CommandHistory.h"
#include "Editor/EditorTypes.h"

namespace Editor
{
	// First click in line mode — standalone node with no wall yet
	struct PlaceNodeCommand : IEditorCommand
	{
		GUID nodeID;
		Core::Vector2 pos;

		PlaceNodeCommand(GUID id, const Core::Vector2& p) : nodeID(id), pos(p) {}

		void Execute(MapData& map) override { map.ReinsertNode(EditorNode{ pos, nodeID }); }
		void Undo(MapData& map) override { map.RemoveNode(nodeID); }
		std::string GetDescription() const override { return "Place Node"; }
		std::optional<GUID> GetRestoredLineTarget() const override { return nodeID; }
	};

	// Second+ click in line mode — right node + wall together (atomic undo)
	struct PlaceLineSegmentCommand : IEditorCommand
	{
		EditorWall wall;
		Core::Vector2 rightNodePos;

		PlaceLineSegmentCommand(const EditorWall& w, const Core::Vector2& pos) : wall(w), rightNodePos(pos) {}

		void Execute(MapData& map) override;
		void Undo(MapData& map) override;
		std::string GetDescription() const override { return "Place Wall"; }
		std::optional<GUID> GetRestoredLineTarget() const override { return wall.rightNodeID; }
		std::optional<GUID> GetRestoredUndoTarget() const override { return wall.leftNodeID; }
		std::optional<GUID> GetRestoredWallID() const override { return wall.wallID; }
	};

	// Sector property changes
	struct SectorFloorHeightCommand : IEditorCommand
	{
		GUID sectorID;
		float oldVal, newVal;

		SectorFloorHeightCommand(GUID id, float old, float new_val) : sectorID(id), oldVal(old), newVal(new_val) {}

		void Execute(MapData& map) override { map.GetSector(sectorID).floorHeight = newVal; }
		void Undo(MapData& map) override { map.GetSector(sectorID).floorHeight = oldVal; }
		std::string GetDescription() const override { return "Change Floor Height"; }
	};

	struct SectorCeilingHeightCommand : IEditorCommand
	{
		GUID sectorID;
		float oldVal, newVal;

		SectorCeilingHeightCommand(GUID id, float old, float new_val) : sectorID(id), oldVal(old), newVal(new_val) {}

		void Execute(MapData& map) override { map.GetSector(sectorID).ceilingHeight = newVal; }
		void Undo(MapData& map) override { map.GetSector(sectorID).ceilingHeight = oldVal; }
		std::string GetDescription() const override { return "Change Ceiling Height"; }
	};

	struct SectorFloorColorCommand : IEditorCommand
	{
		GUID sectorID;
		Core::Color oldVal, newVal;

		SectorFloorColorCommand(GUID id, const Core::Color& old, const Core::Color& new_val) : sectorID(id), oldVal(old), newVal(new_val) {}

		void Execute(MapData& map) override { map.GetSector(sectorID).floorColor = newVal; }
		void Undo(MapData& map) override { map.GetSector(sectorID).floorColor = oldVal; }
		std::string GetDescription() const override { return "Change Floor Color"; }
	};

	struct SectorCeilingColorCommand : IEditorCommand
	{
		GUID sectorID;
		Core::Color oldVal, newVal;

		SectorCeilingColorCommand(GUID id, const Core::Color& old, const Core::Color& new_val) : sectorID(id), oldVal(old), newVal(new_val) {}

		void Execute(MapData& map) override { map.GetSector(sectorID).ceilingColor = newVal; }
		void Undo(MapData& map) override { map.GetSector(sectorID).ceilingColor = oldVal; }
		std::string GetDescription() const override { return "Change Ceiling Color"; }
	};

	// Wall property changes
	struct WallColorCommand : IEditorCommand
	{
		GUID wallID;
		int colorSlot; // 0 = top, 1 = inner, 2 = bottom
		Core::Color oldVal, newVal;

		WallColorCommand(GUID id, int slot, const Core::Color& old, const Core::Color& new_val)
			: wallID(id), colorSlot(slot), oldVal(old), newVal(new_val) {
		}

		void Execute(MapData& map) override
		{
			EditorWall& wall = map.GetWall(wallID);
			if (colorSlot == 0) wall.topColor = newVal;
			else if (colorSlot == 1) wall.innerColor = newVal;
			else if (colorSlot == 2) wall.bottomColor = newVal;
		}

		void Undo(MapData& map) override
		{
			EditorWall& wall = map.GetWall(wallID);
			if (colorSlot == 0) wall.topColor = oldVal;
			else if (colorSlot == 1) wall.innerColor = oldVal;
			else if (colorSlot == 2) wall.bottomColor = oldVal;
		}

		std::string GetDescription() const override { return "Change Wall Color"; }
	};

	// Future: Multi-node drag command (node/wall/sector moves)
	struct NodeDelta
	{
		GUID nodeID;
		Core::Vector2 oldPos, newPos;
	};

	struct GeometryDragCommand : IEditorCommand
	{
		std::vector<NodeDelta> deltas;
		std::string description;

		GeometryDragCommand(const std::string& desc) : description(desc) {}

		void Execute(MapData& map) override;
		void Undo(MapData& map) override;
		std::string GetDescription() const override { return description; }
	};

	// Future: Delete commands
	struct DeleteWallCommand : IEditorCommand
	{
		EditorWall wall;

		DeleteWallCommand(const EditorWall& w) : wall(w) {}

		void Execute(MapData& map) override { map.RemoveWall(wall.wallID); }
		void Undo(MapData& map) override { map.ReinsertWall(wall); }
		std::string GetDescription() const override { return "Delete Wall"; }
	};

	struct DeleteNodeCommand : IEditorCommand
	{
		EditorNode node;

		DeleteNodeCommand(const EditorNode& n) : node(n) {}

		void Execute(MapData& map) override { map.RemoveNode(node.nodeID); }
		void Undo(MapData& map) override { map.ReinsertNode(node); }
		std::string GetDescription() const override { return "Delete Node"; }
	};

	struct DeleteSectorCommand : IEditorCommand
	{
		EditorSector sector;
		std::vector<EditorWall> sectorWalls;

		DeleteSectorCommand(const EditorSector& s, const std::vector<EditorWall>& walls) : sector(s), sectorWalls(walls) {}

		void Execute(MapData& map) override;
		void Undo(MapData& map) override;
		std::string GetDescription() const override { return "Delete Sector"; }
	};

	// Future: Portal assignment
	struct SetPortalCommand : IEditorCommand
	{
		GUID wallID;
		bool oldIsPortal, newIsPortal;
		bool oldIsConnection, newIsConnection;
		GUID oldPortalWallTargetID, newPortalWallTargetID;
		GUID oldPortalTargetSectorID, newPortalTargetSectorID;

		void Execute(MapData& map) override;
		void Undo(MapData& map) override;
		std::string GetDescription() const override { return "Set Portal"; }
	};

	// Future: Composite command for grouping
	struct CompositeCommand : IEditorCommand
	{
		std::vector<std::unique_ptr<IEditorCommand>> steps;
		std::string description;

		CompositeCommand(const std::string& desc) : description(desc) {}

		void Execute(MapData& map) override;
		void Undo(MapData& map) override;
		std::string GetDescription() const override { return description; }
	};
}
