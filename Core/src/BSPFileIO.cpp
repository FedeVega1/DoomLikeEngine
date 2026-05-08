#include "pch.h"

#include <fstream>
#include <string>

#include "Core/VectorMath.h"
#include "Core/WorldTypes.h"
#include "Core/BSPFileIO.h"

namespace Core
{
    template<typename T>
    bool BSPFileReader::ReadValue(std::ifstream& file, T& value)
    {
        return static_cast<bool>(file.read(reinterpret_cast<char*>(&value), sizeof(T)));
    }

    bool BSPFileReader::ReadColor(std::ifstream& file, Color& outColor)
    {
        return ReadValue(file, outColor.r) && ReadValue(file, outColor.g) && ReadValue(file, outColor.b);
    }

    bool BSPFileReader::ReadVector2(std::ifstream& file, Vector2& outVec)
    {
        return ReadValue(file, outVec.x) && ReadValue(file, outVec.y);
    }

    bool BSPFileReader::ReadVector2Int(std::ifstream& file, Vector2Int& outVec)
    {
        return ReadValue(file, outVec.x) && ReadValue(file, outVec.y);
    }

    bool BSPFileReader::ReadWall(std::ifstream& file, Wall& wall)
    {
        if (!ReadVector2(file, wall.leftPoint))  return false;
        if (!ReadVector2(file, wall.rightPoint)) return false;
        if (!ReadColor(file, wall.topColor))     return false;
        if (!ReadColor(file, wall.innerColor))   return false;
        if (!ReadColor(file, wall.bottomColor))  return false;

        uint8_t portalFlags = 0;
        if (!ReadValue(file, portalFlags)) return false;
        wall.isConnection = (portalFlags & BSPFormat::FLAG_CONNECTION) != 0;
        wall.isPortal = (portalFlags & BSPFormat::FLAG_PORTAL) != 0;

        if (!ReadValue(file, wall.portalTargetSectorID)) return false;
        if (!ReadValue(file, wall.portalWallTargetID))   return false;
        if (!ReadValue(file, wall.wallID))               return false;
        if (!ReadValue(file, wall.parentSectorID))       return false;

        return true;
    }

    bool BSPFileReader::ReadSectors(std::ifstream& file, BSPMap& outMap, uint32_t count)
    {
        outMap.sectors.resize(count);
        for (auto& sector : outMap.sectors)
        {
            if (!ReadValue(file, sector.sectorID))       return false;
            if (!ReadValue(file, sector.floorHeight))    return false;
            if (!ReadValue(file, sector.ceillingHeight)) return false;
            if (!ReadColor(file, sector.floorColor))     return false;
            if (!ReadColor(file, sector.ceillingColor))  return false;
        }
        return true;
    }

    bool BSPFileReader::ReadSubSectors(std::ifstream& file, BSPMap& outMap, uint32_t count)
    {
        outMap.subSectors.resize(count);
        for (auto& subSector : outMap.subSectors)
        {
            if (!ReadValue(file, subSector.subSectorID)) return false;

            uint32_t wallCount = 0;
            if (!ReadValue(file, wallCount)) return false;

            subSector.walls.resize(wallCount);
            for (auto& wall : subSector.walls)
            {
                if (!ReadWall(file, wall)) return false;
            }
        }
        return true;
    }

    std::unique_ptr<BSPNode> BSPFileReader::ReadNodeRecursive(std::ifstream& file, BSPMap& outMap, BSPNode* parent)
    {
        auto node = std::make_unique<BSPNode>();
        node->parentNode = parent;

        if (!ReadValue(file, node->childFlag))                  return nullptr;
        if (!ReadValue(file, node->nodeID))                     return nullptr;
        if (!ReadVector2Int(file, node->splitter.startPoint))   return nullptr;
        if (!ReadVector2Int(file, node->splitter.segment))      return nullptr;
        if (!ReadVector2(file, node->splitter.dir))             return nullptr;

        // parentID is informational only — we already have the pointer
        uint32_t parentID = NULL_ID_32;
        if (!ReadValue(file, parentID)) return nullptr;

        uint32_t subSectorID = NULL_ID_32;
        if (!ReadValue(file, subSectorID)) return nullptr;

        if (!ReadVector2Int(file, node->boundingBox.topPoint))    return nullptr;
        if (!ReadVector2Int(file, node->boundingBox.bottomPoint)) return nullptr;

        if (subSectorID != NULL_ID_32)
        {
            for (auto& ss : outMap.subSectors)
            {
                if (ss.subSectorID != subSectorID) continue;
                node->subSector = &ss;
                break;
            }
        }

        // childrenFlags: 0x01 = has front, 0x02 = has back
        uint8_t childrenFlags = 0;
        if (!ReadValue(file, childrenFlags)) return nullptr;

        if (childrenFlags & 0x01)
        {
            node->frontNode = ReadNodeRecursive(file, outMap, node.get());
            if (!node->frontNode) return nullptr;
        }

        if (childrenFlags & 0x02)
        {
            node->backNode = ReadNodeRecursive(file, outMap, node.get());
            if (!node->backNode) return nullptr;
        }

        return node;
    }

    bool BSPFileReader::ReadBSPNodes(std::ifstream& file, BSPMap& outMap)
    {
        outMap.rootNode = ReadNodeRecursive(file, outMap, nullptr);
        return outMap.rootNode != nullptr;
    }

    bool BSPFileReader::Read(const std::string& filePath, BSPMap& outMap)
    {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) return false;

        char magic[4] = {};
        file.read(magic, 4);
        if (magic[0] != BSPFormat::MAGIC[0] || magic[1] != BSPFormat::MAGIC[1] ||
            magic[2] != BSPFormat::MAGIC[2] || magic[3] != BSPFormat::MAGIC[3])
            return false;

        uint8_t vMajor = 0, vMinor = 0;
        if (!ReadValue(file, vMajor) || !ReadValue(file, vMinor)) return false;
        if (vMajor != BSPFormat::VERSION_MAJOR || vMinor != BSPFormat::VERSION_MINOR) return false;

        uint32_t sectorCount = 0;
        if (!ReadValue(file, sectorCount)) return false;
        if (!ReadSectors(file, outMap, sectorCount)) return false;

        uint32_t subSectorCount = 0;
        if (!ReadValue(file, subSectorCount)) return false;
        if (!ReadSubSectors(file, outMap, subSectorCount)) return false;

        if (!ReadBSPNodes(file, outMap)) return false;

        outMap.LinkPortals();
        return true;
    }
}
