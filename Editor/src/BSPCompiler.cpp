#include "pch.h"

#include <fstream>
#include <algorithm>
#include <cmath>

#include "Core/BSPFileIO.h"
#include "Editor/EditorTypes.h"
#include "Editor/BSPCompiler.h"

namespace Editor
{
    const Core::Vector2Int BSPCompiler::DIRS[6] =
    {
        Core::Vector2Int::RIGHT, Core::Vector2Int::LEFT,
        Core::Vector2Int::UP, Core::Vector2Int::DOWN,
        Core::Vector2Int::ONE, -Core::Vector2Int::ONE
    };

    Core::Sector EditorSectorToSector(const EditorSector& eSector)
    {
        Core::Sector sector;
        sector.sectorID = eSector.sectorID;
        sector.floorHeight = eSector.floorHeight;
        sector.ceillingHeight = eSector.ceillingHeight;
        sector.floorColor = eSector.floorColor;
        sector.ceillingColor = eSector.ceillingColor;

        return sector;
    }

    Core::Wall EditorWallToWall(const EditorWall& eWall, const uint32_t& editorSectorID)
    {
        Core::Wall wall;
        wall.leftPoint = eWall.leftPoint;
        wall.rightPoint = eWall.rightPoint;
        wall.topColor = eWall.topColor;
        wall.innerColor = eWall.innerColor;
        wall.bottomColor = eWall.bottomColor;
        wall.isPortal = eWall.isPortal;
        wall.isConnection = eWall.isConnection;
        wall.wallID = eWall.wallID;
        wall.portalWallTargetID = eWall.portalWallTargetID;
        wall.portalTargetSectorID = eWall.portalTargetSectorID;
        wall.parentSectorID = editorSectorID;

        return wall;
    }

    template<typename T>
    void BSPCompiler::WriteValue(std::ofstream& file, const T& value)
    {
        file.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    void BSPCompiler::WriteColor(std::ofstream& file, const Core::Color& color)
    {
        WriteValue(file, color.r);
        WriteValue(file, color.g);
        WriteValue(file, color.b);
    }

    void BSPCompiler::WriteVector2(std::ofstream& file, const Core::Vector2& v)
    {
        WriteValue(file, v.x);
        WriteValue(file, v.y);
    }

    void BSPCompiler::WriteVector2Int(std::ofstream& file, const Core::Vector2Int& v)
    {
        WriteValue(file, v.x);
        WriteValue(file, v.y);
    }

    Core::BoundingBox BSPCompiler::CalculateBounds(const std::vector<Core::Wall>& walls) const
    {
        Core::BoundingBox box;
        box.topPoint = Core::Vector2Int(INT_MAX, INT_MAX);
        box.bottomPoint = Core::Vector2Int(INT_MIN, INT_MIN);

        for (const Core::Wall& wall : walls)
        {
            box.topPoint.x = std::min(box.topPoint.x, static_cast<int>(std::min(wall.leftPoint.x, wall.rightPoint.x)));
            box.topPoint.y = std::min(box.topPoint.y, static_cast<int>(std::min(wall.leftPoint.y, wall.rightPoint.y)));
            box.bottomPoint.x = std::max(box.bottomPoint.x, static_cast<int>(std::max(wall.leftPoint.x, wall.rightPoint.x)));
            box.bottomPoint.y = std::max(box.bottomPoint.y, static_cast<int>(std::max(wall.leftPoint.y, wall.rightPoint.y)));
        }

        return box;
    }

    void BSPCompiler::SimulateSplits(const std::vector<Core::Wall>& walls, const Core::Splitter& splitter, int& outFront,
        int& outBack, int& outSplit) const
    {
        outFront = outBack = outSplit = 0;

        for (const auto& wall : walls)
        {
            float crossLeft = Core::Vector2::Cross(
                Core::Vector2(static_cast<float>(splitter.segment.x), static_cast<float>(splitter.segment.y)),
                wall.leftPoint - Core::Vector2(static_cast<float>(splitter.startPoint.x), static_cast<float>(splitter.startPoint.y)));

            float crossRight = Core::Vector2::Cross(
                Core::Vector2(static_cast<float>(splitter.segment.x), static_cast<float>(splitter.segment.y)),
                wall.rightPoint - Core::Vector2(static_cast<float>(splitter.startPoint.x), static_cast<float>(splitter.startPoint.y)));

            if (crossLeft > 0 && crossRight > 0) { outFront++; continue; }
            if (crossLeft < 0 && crossRight < 0) { outBack++; continue; }
            outSplit++;
        }
    }

    Core::Splitter BSPCompiler::SelectSplitter(const std::vector<Core::Wall>& walls, const Core::BoundingBox& bounds) const
    {
        Core::Splitter bestSplitter{};
        int bestScore = INT_MIN;

        auto evaluate = [&](const Core::Splitter& candidate)
        {
            int front, back, split;
            SimulateSplits(walls, candidate, front, back, split);
            int score = -(std::abs(front - back) + (split * 2));
            if (score > bestScore)
            {
                bestScore = score;
                bestSplitter = candidate;
            }
        };

        // Test each wall as a potential splitter
        for (const auto& wall : walls)
        {
            Core::Vector2Int seg = Core::Vector2Int(static_cast<int>(wall.rightPoint.x - wall.leftPoint.x),
                static_cast<int>(wall.rightPoint.y - wall.leftPoint.y));

            Core::Vector2 dir = Core::Vector2::Normalize(Core::Vector2(static_cast<float>(seg.x), static_cast<float>(seg.y)));

            Core::Splitter candidate;
            candidate.startPoint = Core::Vector2Int(static_cast<int>(wall.leftPoint.x), static_cast<int>(wall.leftPoint.y));
            candidate.segment = seg;
            candidate.dir = dir;

            evaluate(candidate);
        }

        // Fallback: try 6 axis-aligned splitters from bounding box center
        if (bestScore == INT_MIN)
        {
            static constexpr int SPLITTER_LENGTH = 30;

            Core::Vector2Int center = Core::Vector2Int((bounds.topPoint.x + bounds.bottomPoint.x) / 2, (bounds.topPoint.y + bounds.bottomPoint.y) / 2);

            for (const Core::Vector2Int& d : DIRS)
            {
                Core::Vector2 dir = Core::Vector2::Normalize(Core::Vector2(static_cast<float>(d.x), static_cast<float>(d.y)));
                Core::Splitter candidate;
                candidate.startPoint = center;
                candidate.segment = Core::Vector2Int(d.x * SPLITTER_LENGTH, d.y * SPLITTER_LENGTH);
                candidate.dir = dir;
                evaluate(candidate);
            }
        }

        return bestSplitter;
    }

    std::vector<Core::Vector2> BSPCompiler::BuildPointChain(const std::vector<Core::Wall>& walls) const
    {
        std::vector<Core::Vector2> points;
        std::vector<bool> used = std::vector<bool>(walls.size(), false);

        points.push_back(walls[0].leftPoint);
        Core::Vector2 current = walls[0].rightPoint;

        points.push_back(current);
        used[0] = true;

        for (size_t iter = 1; iter < walls.size(); iter++)
        {
            bool found = false;
            for (size_t i = 1; i < walls.size(); i++)
            {
                if (used[i]) continue;
                if ((walls[i].leftPoint - current).Magnitude() < Core::K_EPSILON)
                {
                    current = walls[i].rightPoint;
                    points.push_back(current);
                    used[i] = true;
                    found = true;
                    break;
                }
            }
            if (!found) return {};
        }

        return points;
    }

    bool BSPCompiler::IsConvex(const std::vector<Core::Wall>& walls) const
    {
        if (walls.size() < 2) return true;

        std::vector<Core::Vector2> points = BuildPointChain(walls);
        if (points.empty() || points.size() <= walls.size()) return false;

        size_t n = points.size();
        for (size_t i = 0; i < n; i++)
        {
            Core::Vector2 a = points[i];
            Core::Vector2 b = points[(i + 1) % n];
            Core::Vector2 c = points[(i + 2) % n];

            float cross = Core::Vector2::Cross(b - a, c - b);
            if (cross > Core::K_EPSILON) return false;
        }

        return true;
    }

    bool BSPCompiler::SplitWall(const Core::Wall& wall, const Core::Splitter& splitter, Core::Wall& outFront, Core::Wall& outBack) const
    {
        Core::Vector2 seg = Core::Vector2(static_cast<float>(splitter.segment.x), static_cast<float>(splitter.segment.y));
        Core::Vector2 origin = Core::Vector2(static_cast<float>(splitter.startPoint.x), static_cast<float>(splitter.startPoint.y));
        Core::Vector2 toLeft = wall.leftPoint - origin;
        Core::Vector2 toRight = wall.rightPoint - origin;

        float crossLeft = Core::Vector2::Cross(seg, toLeft);
        float crossRight = Core::Vector2::Cross(seg, toRight);

        if ((crossLeft >= 0 && crossRight >= 0) || (crossLeft <= 0 && crossRight <= 0))
            return false;

        float t = crossLeft / (crossLeft - crossRight);
        Core::Vector2 intersection = Core::Vector2(wall.leftPoint.x + t * (wall.rightPoint.x - wall.leftPoint.x),
            wall.leftPoint.y + t * (wall.rightPoint.y - wall.leftPoint.y));

        outFront = wall;
        outBack = wall;

        if (crossLeft > 0)
        {
            outFront.leftPoint = wall.leftPoint;
            outFront.rightPoint = intersection;
            outBack.leftPoint = intersection;
            outBack.rightPoint = wall.rightPoint;
            return true;
        }

        outBack.leftPoint = wall.leftPoint;
        outBack.rightPoint = intersection;
        outFront.leftPoint = intersection;
        outFront.rightPoint = wall.rightPoint;

        return true;
    }

    bool BSPCompiler::IsInverseWall(const Core::Wall& a, const Core::Wall& b)
    {
        return (a.leftPoint - b.rightPoint).Magnitude() < Core::K_EPSILON &&
            (a.rightPoint - b.leftPoint).Magnitude() < Core::K_EPSILON;
    }

    void BSPCompiler::PartitionWalls(const std::vector<Core::Wall>& walls, const Core::Splitter& splitter, std::vector<Core::Wall>& outFront,
        std::vector<Core::Wall>& outBack) const
    {
        const Core::Wall* lastConn = nullptr;
        Core::Vector2 seg = Core::Vector2(static_cast<float>(splitter.segment.x), static_cast<float>(splitter.segment.y));
        Core::Vector2 origin = Core::Vector2(static_cast<float>(splitter.startPoint.x), static_cast<float>(splitter.startPoint.y));

        for (const Core::Wall& wall : walls)
        {
            Core::Vector2 effectiveLeft = wall.leftPoint;
            Core::Vector2 effectiveRight = wall.rightPoint;

            if (wall.isConnection && lastConn != nullptr && IsInverseWall(wall, *lastConn))
            {
                Core::Vector2 dir = Core::Vector2::Normalize(wall.rightPoint - wall.leftPoint);
                Core::Vector2 perp(-dir.y, dir.x);
                effectiveLeft = wall.leftPoint + perp;
                effectiveRight = wall.rightPoint + perp;
            }

            float crossL = Core::Vector2::Cross(seg, effectiveLeft - origin);
            float crossR = Core::Vector2::Cross(seg, effectiveRight - origin);

            if (crossL > 0 && crossR > 0) { outFront.push_back(wall); }
            else if (crossL < 0 && crossR < 0) { outBack.push_back(wall); }
            else
            {
                Core::Wall front, back;
                if (SplitWall(wall, splitter, front, back))
                {
                    outFront.push_back(front);
                    outBack.push_back(back);
                }
                else
                {
                    if (splitter.VectorInFront(wall.leftPoint)) outFront.push_back(wall);
                    else outBack.push_back(wall);
                }
            }

            if (wall.isConnection) lastConn = &wall;
        }
    }

    std::unique_ptr<Core::BSPNode> BSPCompiler::GenerateLeafs(std::vector<Core::Wall> walls, Core::BSPNode* parentNode, uint8_t childFlag)
    {
        auto node = std::make_unique<Core::BSPNode>();
        node->nodeID = nextNodeID++;
        node->childFlag = childFlag;
        node->parentNode = parentNode;
        node->boundingBox = CalculateBounds(walls);

        if (IsConvex(walls))
        {
            Core::SubSector subSector;
            subSector.subSectorID = nextSubSectorID++;
            subSector.walls = std::move(walls);
            compiledSubSectors.push_back(std::move(subSector));
            node->subSector = &compiledSubSectors.back();
            return node;
        }

        Core::Splitter splitter = SelectSplitter(walls, node->boundingBox);
        node->splitter = splitter;

        std::vector<Core::Wall> frontWalls, backWalls;
        PartitionWalls(walls, splitter, frontWalls, backWalls);

        if (!frontWalls.empty())
            node->frontNode = GenerateLeafs(std::move(frontWalls), node.get(), Core::BSPNode::FLAG_FRONT);

        if (!backWalls.empty())
            node->backNode = GenerateLeafs(std::move(backWalls), node.get(), Core::BSPNode::FLAG_BACK);

        return node;
    }

    CompilerResult BSPCompiler::BuildBSP(const std::vector<EditorSector>& editorSectors)
    {
        CompilerResult result;
        compiledSubSectors.clear();
        nextNodeID = 0;
        nextSubSectorID = 0;

        std::vector<Core::Wall> allWalls;
        for (const EditorSector& editorSector : editorSectors)
        {
            result.sectors.push_back(EditorSectorToSector(editorSector));

            for (const EditorWall& editorWall : editorSector.walls)
                allWalls.push_back(EditorWallToWall(editorWall, editorSector.sectorID));
        }

        if (allWalls.empty())
        {
            result.success = false;
            return result;
        }

        result.rootNode = GenerateLeafs(std::move(allWalls), nullptr, Core::BSPNode::FLAG_ROOT);
        result.subSectors = std::move(compiledSubSectors);
        result.success = result.rootNode != nullptr;
        return result;
    }

    void BSPCompiler::WriteNodeRecursive(std::ofstream& file, const Core::BSPNode* node) const
    {
        WriteValue(file, node->childFlag);
        WriteValue(file, node->nodeID);
        WriteVector2Int(file, node->splitter.startPoint);
        WriteVector2Int(file, node->splitter.segment);
        WriteVector2(file, node->splitter.dir);

        uint32_t parentID = node->parentNode ? node->parentNode->nodeID : Core::NULL_ID_32;
        uint32_t subSectorID = node->subSector ? node->subSector->subSectorID : Core::NULL_ID_32;
        WriteValue(file, parentID);
        WriteValue(file, subSectorID);

        WriteVector2Int(file, node->boundingBox.topPoint);
        WriteVector2Int(file, node->boundingBox.bottomPoint);

        uint8_t childrenFlags = 0;
        if (node->frontNode) childrenFlags |= 0x01;
        if (node->backNode)  childrenFlags |= 0x02;
        WriteValue(file, childrenFlags);

        if (node->frontNode) WriteNodeRecursive(file, node->frontNode.get());
        if (node->backNode)  WriteNodeRecursive(file, node->backNode.get());
    }

    bool BSPCompiler::WriteResult(const CompilerResult& result, const std::string& outputPath) const
    {
        std::ofstream file(outputPath, std::ios::binary | std::ios::trunc);
        if (!file.is_open()) return false;

        file.write(Core::BSPFormat::MAGIC, 4);
        WriteValue(file, Core::BSPFormat::VERSION_MAJOR);
        WriteValue(file, Core::BSPFormat::VERSION_MINOR);

        uint32_t sectorCount = static_cast<uint32_t>(result.sectors.size());
        WriteValue(file, sectorCount);
        for (const auto& sector : result.sectors)
        {
            WriteValue(file, sector.sectorID);
            WriteValue(file, sector.floorHeight);
            WriteValue(file, sector.ceillingHeight);
            WriteColor(file, sector.floorColor);
            WriteColor(file, sector.ceillingColor);
        }

        uint32_t subSectorCount = static_cast<uint32_t>(result.subSectors.size());
        WriteValue(file, subSectorCount);
        for (const auto& subSector : result.subSectors)
        {
            WriteValue(file, subSector.subSectorID);
            uint32_t wallCount = static_cast<uint32_t>(subSector.walls.size());
            WriteValue(file, wallCount);
            for (const auto& wall : subSector.walls)
            {
                WriteVector2(file, wall.leftPoint);
                WriteVector2(file, wall.rightPoint);
                WriteColor(file, wall.topColor);
                WriteColor(file, wall.innerColor);
                WriteColor(file, wall.bottomColor);

                uint8_t flags = 0;
                if (wall.isConnection) flags |= Core::BSPFormat::FLAG_CONNECTION;
                if (wall.isPortal)     flags |= Core::BSPFormat::FLAG_PORTAL;
                WriteValue(file, flags);

                WriteValue(file, wall.portalTargetSectorID);
                WriteValue(file, wall.portalWallTargetID);
                WriteValue(file, wall.wallID);
                WriteValue(file, wall.parentSectorID);
            }
        }

        WriteNodeRecursive(file, result.rootNode.get());
        return true;
    }

    bool BSPCompiler::Compile(const std::vector<EditorSector>& editorSectors, const std::string& outputPath)
    {
        CompilerResult result = BuildBSP(editorSectors);
        if (!result.success) return false;
        return WriteResult(result, outputPath);
    }
}
