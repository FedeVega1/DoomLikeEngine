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
        wallAdjacency[wall.leftNodeID].push_back(wall.wallID);
        wallAdjacency[wall.rightNodeID].push_back(wall.wallID);
        return wall.wallID;
    }

    GUID MapData::AddSector(EditorSector& sector)
    {
        sector.sectorID = ++sectorCounter;
        sectors.emplace(sector.sectorID, sector);
        for (GUID wallID : sector.walls)
            usedWalls.insert(wallID);
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

    void MapData::RemoveWall(GUID id)
    {
        auto it = walls.find(id);
        if (it == walls.end()) return;

        const EditorWall& wall = it->second;
        auto& leftAdj = wallAdjacency[wall.leftNodeID];
        leftAdj.erase(std::remove(leftAdj.begin(), leftAdj.end(), id), leftAdj.end());
        auto& rightAdj = wallAdjacency[wall.rightNodeID];
        rightAdj.erase(std::remove(rightAdj.begin(), rightAdj.end(), id), rightAdj.end());

        walls.erase(it);
    }

    void MapData::RemoveSector(GUID id)
    {
        auto it = sectors.find(id);
        if (it == sectors.end()) return;

        for (GUID wallID : it->second.walls)
            usedWalls.erase(wallID);

        sectors.erase(it);
    }

    void MapData::ReinsertWall(const EditorWall& wall)
    {
        walls.emplace(wall.wallID, wall);
        wallAdjacency[wall.leftNodeID].push_back(wall.wallID);
        wallAdjacency[wall.rightNodeID].push_back(wall.wallID);
    }

    void MapData::ReinsertSector(const EditorSector& sect)
    {
        sectors.emplace(sect.sectorID, sect);
        for (GUID wallID : sect.walls)
            usedWalls.insert(wallID);
    }

    std::optional<GUID> MapData::FindWallBetweenNodes(GUID nodeA, GUID nodeB) const
    {
        auto it = wallAdjacency.find(nodeA);
        if (it == wallAdjacency.end()) return std::nullopt;

        for (GUID wallID : it->second)
        {
            const EditorWall& wall = walls.at(wallID);
            if ((wall.leftNodeID == nodeA && wall.rightNodeID == nodeB) ||
                (wall.leftNodeID == nodeB && wall.rightNodeID == nodeA))
                return wallID;
        }

        return std::nullopt;
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
        GUID previousNode = Core::NULL_ID_32;
        std::vector<GUID> result;
        std::unordered_set<GUID> visitedWalls;

        auto getBestWall = [&](const std::vector<GUID>& candidates, bool newOnly) -> GUID
        {
            if (previousNode == Core::NULL_ID_32)
            {
                for (GUID wallID : candidates)
                {
                    if (visitedWalls.count(wallID)) continue;
                    if (newOnly && usedWalls.count(wallID)) continue;
                    return wallID;
                }
                return Core::NULL_ID_32;
            }

            const Core::Vector2& currPos = nodes.at(currentNode).pos;
            const Core::Vector2& prevPos = nodes.at(previousNode).pos;
            float incomingAngle = std::atan2(prevPos.y - currPos.y, prevPos.x - currPos.x);

            GUID bestWall = Core::NULL_ID_32;
            float bestRotation = std::numeric_limits<float>::max();

            for (GUID wallID : candidates)
            {
                if (visitedWalls.count(wallID)) continue;
                if (newOnly && usedWalls.count(wallID)) continue;

                const EditorWall& wall = walls.at(wallID);
                GUID nextNode = (wall.leftNodeID == currentNode) ? wall.rightNodeID : wall.leftNodeID;
                const Core::Vector2& nextPos = nodes.at(nextNode).pos;

                float wallAngle = std::atan2(nextPos.y - currPos.y, nextPos.x - currPos.x);
                float rotation = wallAngle - incomingAngle;
                if (rotation <= 0.f) rotation += 6.28318530f;

                if (rotation < bestRotation)
                {
                    bestRotation = rotation;
                    bestWall = wallID;
                }
            }

            return bestWall;
        };

        auto advance = [&](GUID wallID)
        {
            const EditorWall& wall = walls.at(wallID);
            result.push_back(wallID);
            visitedWalls.insert(wallID);
            previousNode = currentNode;
            currentNode = (wall.leftNodeID == currentNode) ? wall.rightNodeID : wall.leftNodeID;
        };

        do
        {
            auto adjIt = wallAdjacency.find(currentNode);
            if (adjIt == wallAdjacency.end()) break;

            const std::vector<GUID>& candidates = adjIt->second;

            GUID next = getBestWall(candidates, true);
            if (next == Core::NULL_ID_32)
                next = getBestWall(candidates, false);

            if (next == Core::NULL_ID_32) break;

            advance(next);

        } while (currentNode != nodeID);

        return result;
    }
}
