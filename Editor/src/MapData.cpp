#include "pch.h"
#include "Editor/EditorTypes.h"

namespace Editor
{
    GUID MapData::AddNode(const Core::Vector2& pos)
    {
        EditorNode newNode = { pos, ++nodeCounter };
        nodes.emplace(newNode.nodeID, newNode);
        return newNode.nodeID;
    }

    GUID MapData::AddWall(EditorWall& wall)
    {
        wall.wallID = ++wallCounter;
        walls.emplace(wall.wallID, wall);
        return wall.wallID;
    }

    GUID MapData::AddSector(EditorSector& sector)
    {
        sector.sectorID = ++sectorCounter;
        sectors.emplace(sector.sectorID, sector);
        return sector.sectorID;
    }

    void MapData::ForEachNode(const std::function<void(const EditorNode&)>& callback) const
    {
        for (const auto& [id, node] : nodes)
        {
            if (callback == nullptr) continue;
            callback(node);
        }
    }

    void MapData::ForEachWall(const std::function<void(const EditorWall&)>& callback) const
    {
        for (const auto& [id, wall] : walls)
        {
            if (callback == nullptr) continue;
            callback(wall);
        }
    }

    void MapData::ForEachSector(const std::function<void(const EditorSector&)>& callback) const
    {
        for (const auto& [id, sector] : sectors)
        {
            if (callback == nullptr) continue;
            callback(sector);
        }
    }

    void MapData::RefreshWallBoundsForNode(GUID nodeID)
    {
        for (auto& [wallID, wall] : walls)
        {
            if (wall.leftNodeID == nodeID || wall.rightNodeID == nodeID)
            {
                const EditorNode& leftNode = GetNode(wall.leftNodeID);
                const EditorNode& rightNode = GetNode(wall.rightNodeID);
                wall.UpdateBounds(leftNode.pos, rightNode.pos);
            }
        }
    }

    std::optional<std::reference_wrapper<const EditorWall>> MapData::FindWallByNodeID(GUID id) const
    {
        for (auto& [wallID, wall] : walls)
        {
            if (wall.leftNodeID != id && wall.rightNodeID != id) continue;
            return std::cref(wall);
        }

        return std::nullopt;
    }

    std::vector<GUID> MapData::FindSectorWallsByFirstNode(GUID nodeID) const
    {
        GUID currentNode = nodeID;
        std::vector<GUID> wallVector;

        do
        {
            auto currentWall = FindWallByNodeID(nodeID);
            if (!currentWall.has_value()) continue;

            const EditorWall& wall = currentWall.value().get();
            wallVector.push_back(wall.wallID);

            currentNode = wall.rightNodeID;
        } while (currentNode != nodeID);

        return wallVector;
    }
}
