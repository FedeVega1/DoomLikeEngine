#include "pch.h"
#include <fstream>

#include "Editor/EditorTypes.h"
#include "Editor/MapFileIO.h"

namespace Editor
{
    template<typename T>
    bool MapFileIO::ReadValue(std::ifstream& file, T& value)
    {
        return static_cast<bool>(file.read(reinterpret_cast<char*>(&value), sizeof(T)));
    }

    template<typename T>
    void MapFileIO::WriteValue(std::ofstream& file, const T& value)
    {
        file.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    bool MapFileIO::ReadColor(std::ifstream& file, Core::Color& outColor)
    {
        return ReadValue(file, outColor.r) && ReadValue(file, outColor.g) && ReadValue(file, outColor.b);
    }

    void MapFileIO::WriteColor(std::ofstream& file, const Core::Color& color)
    {
        WriteValue(file, color.r);
        WriteValue(file, color.g);
        WriteValue(file, color.b);
    }

    bool MapFileIO::ReadWall(std::ifstream& file, EditorWall& wall)
    {
        float lx, ly, rx, ry;
        if (!ReadValue(file, lx) || !ReadValue(file, ly)) return false;
        if (!ReadValue(file, rx) || !ReadValue(file, ry)) return false;
        //wall.leftPoint = Core::Vector2(lx, ly);
        //wall.rightPoint = Core::Vector2(rx, ry);

        if (!ReadColor(file, wall.topColor)) return false;
        if (!ReadColor(file, wall.innerColor)) return false;
        if (!ReadColor(file, wall.bottomColor)) return false;

        uint8_t flags = 0;
        if (!ReadValue(file, flags)) return false;
        wall.isConnection = (flags & MapFormat::FLAG_CONNECTION) != 0;
        wall.isPortal = (flags & MapFormat::FLAG_PORTAL) != 0;

        if (!ReadValue(file, wall.portalTargetSectorID)) return false;
        if (!ReadValue(file, wall.portalWallTargetID)) return false;
        if (!ReadValue(file, wall.wallID)) return false;

        return true;
    }

    bool MapFileIO::ReadSector(std::ifstream& file, EditorSector& sector)
    {
        if (!ReadValue(file, sector.sectorID)) return false;
        if (!ReadValue(file, sector.floorHeight)) return false;
        if (!ReadValue(file, sector.ceillingHeight)) return false;
        if (!ReadColor(file, sector.floorColor)) return false;
        if (!ReadColor(file, sector.ceillingColor)) return false;

        uint32_t wallCount = 0;
        if (!ReadValue(file, wallCount)) return false;

        sector.walls.resize(wallCount);
        //for (EditorWall& wall : sector.walls)
        //{
        //    if (!ReadWall(file, wall)) 
        //        return false;
        //}

        return true;
    }

    void MapFileIO::WriteWall(std::ofstream& file, const EditorWall& wall)
    {
        //WriteValue(file, wall.leftPoint.x);
        //WriteValue(file, wall.leftPoint.y);
        //WriteValue(file, wall.rightPoint.x);
        //WriteValue(file, wall.rightPoint.y);
        WriteColor(file, wall.topColor);
        WriteColor(file, wall.innerColor);
        WriteColor(file, wall.bottomColor);

        uint8_t flags = 0;
        if (wall.isConnection) flags |= MapFormat::FLAG_CONNECTION;
        if (wall.isPortal) flags |= MapFormat::FLAG_PORTAL;
        WriteValue(file, flags);

        WriteValue(file, wall.portalTargetSectorID);
        WriteValue(file, wall.portalWallTargetID);
        WriteValue(file, wall.wallID);
    }

    void MapFileIO::WriteSector(std::ofstream& file, const EditorSector& sector)
    {
        WriteValue(file, sector.sectorID);
        WriteValue(file, sector.floorHeight);
        WriteValue(file, sector.ceillingHeight);
        WriteColor(file, sector.floorColor);
        WriteColor(file, sector.ceillingColor);

        uint32_t wallCount = static_cast<uint32_t>(sector.walls.size());
        WriteValue(file, wallCount);
        //for (const EditorWall& wall : sector.walls)
        //{
        //    WriteWall(file, wall);
        //}
    }

    bool MapFileIO::Read(const std::string& filePath, std::vector<EditorSector>& outSectors)
    {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) return false;

        char magic[4] = {};
        file.read(magic, 4);
        if (magic[0] != MapFormat::MAGIC[0] || magic[1] != MapFormat::MAGIC[1] || magic[2] != MapFormat::MAGIC[2] || magic[3] != MapFormat::MAGIC[3])
            return false;

        uint8_t vMajor = 0, vMinor = 0;
        if (!ReadValue(file, vMajor) || !ReadValue(file, vMinor)) return false;
        if (vMajor != MapFormat::VERSION_MAJOR || vMinor != MapFormat::VERSION_MINOR) return false;

        uint32_t sectorCount = 0;
        if (!ReadValue(file, sectorCount)) return false;

        outSectors.resize(sectorCount);
        for (EditorSector& sector : outSectors)
        {
            if (!ReadSector(file, sector)) 
                return false;
        }

        return true;
    }

    bool MapFileIO::Write(const std::string& filePath, const std::vector<EditorSector>& sectors)
    {
        std::ofstream file(filePath, std::ios::binary | std::ios::trunc);
        if (!file.is_open()) return false;

        file.write(MapFormat::MAGIC, 4);
        WriteValue(file, MapFormat::VERSION_MAJOR);
        WriteValue(file, MapFormat::VERSION_MINOR);

        uint32_t sectorCount = static_cast<uint32_t>(sectors.size());
        WriteValue(file, sectorCount);
        for (const EditorSector& sector : sectors)
        {
            WriteSector(file, sector);
        }

        return true;
    }
}
