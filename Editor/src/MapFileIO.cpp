#include "pch.h"
#include <fstream>

#include "Editor/EditorTypes.h"
#include "Editor/MapFileIO.h"

namespace Editor
{
    template<typename T>
    std::optional<T> MapFileIO::ReadValue(std::ifstream& file)
    {
        T value{};
        if (!file.read(reinterpret_cast<char*>(&value), sizeof(T))) return std::nullopt;
        return value;
    }

    template<typename T>
    void MapFileIO::WriteValue(std::ofstream& file, const T& value)
    {
        file.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    std::optional<Core::Color> MapFileIO::ReadColor(std::ifstream& file)
    {
        auto r = ReadValue<uint8_t>(file);
        auto g = ReadValue<uint8_t>(file);
        auto b = ReadValue<uint8_t>(file);
        if (!r || !g || !b) return std::nullopt;
        return Core::Color(*r, *g, *b);
    }

    void MapFileIO::WriteColor(std::ofstream& file, const Core::Color& color)
    {
        WriteValue(file, color.r);
        WriteValue(file, color.g);
        WriteValue(file, color.b);
    }

    std::optional<EditorWall> MapFileIO::ReadWall(std::ifstream& file)
    {
        auto lx = ReadValue<float>(file); auto ly = ReadValue<float>(file);
        auto rx = ReadValue<float>(file); auto ry = ReadValue<float>(file);
        if (!lx || !ly || !rx || !ry) return std::nullopt;

        auto topColor = ReadColor(file);
        auto innerColor = ReadColor(file);
        auto bottomColor = ReadColor(file);
        if (!topColor || !innerColor || !bottomColor) return std::nullopt;

        auto flags = ReadValue<uint8_t>(file);
        if (!flags) return std::nullopt;

        auto portalTargetSectorID = ReadValue<GUID>(file);
        auto portalWallTargetID = ReadValue<GUID>(file);
        auto wallID = ReadValue<GUID>(file);
        if (!portalTargetSectorID || !portalWallTargetID || !wallID) return std::nullopt;

        EditorWall wall{};
        //wall.leftPoint = Core::Vector2(*lx, *ly);
        //wall.rightPoint = Core::Vector2(*rx, *ry);
        wall.topColor = *topColor;
        wall.innerColor = *innerColor;
        wall.bottomColor = *bottomColor;
        wall.isConnection = (*flags & MapFormat::FLAG_CONNECTION) != 0;
        wall.isPortal = (*flags & MapFormat::FLAG_PORTAL) != 0;
        wall.portalTargetSectorID = *portalTargetSectorID;
        wall.portalWallTargetID = *portalWallTargetID;
        wall.wallID = *wallID;
        return wall;
    }

    std::optional<EditorSector> MapFileIO::ReadSector(std::ifstream& file)
    {
        auto sectorID = ReadValue<GUID>(file);
        auto floorHeight = ReadValue<float>(file);
        auto ceilingHeight = ReadValue<float>(file);
        if (!sectorID || !floorHeight || !ceilingHeight) return std::nullopt;

        auto floorColor = ReadColor(file);
        auto ceilingColor = ReadColor(file);
        if (!floorColor || !ceilingColor) return std::nullopt;

        auto wallCount = ReadValue<uint32_t>(file);
        if (!wallCount) return std::nullopt;

        EditorSector sector{};
        sector.sectorID = *sectorID;
        sector.floorHeight = *floorHeight;
        sector.ceilingHeight = *ceilingHeight;
        sector.floorColor = *floorColor;
        sector.ceilingColor = *ceilingColor;
        sector.walls.resize(*wallCount);
        //for (GUID& wallID : sector.walls)
        //{
        //    auto wall = ReadWall(file);
        //    if (!wall) return std::nullopt;
        //    wallID = wall->wallID;
        //}
        return sector;
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
        WriteValue(file, sector.ceilingHeight);
        WriteColor(file, sector.floorColor);
        WriteColor(file, sector.ceilingColor);

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

        auto vMajor = ReadValue<uint8_t>(file);
        auto vMinor = ReadValue<uint8_t>(file);
        if (!vMajor || !vMinor) return false;
        if (*vMajor != MapFormat::VERSION_MAJOR || *vMinor != MapFormat::VERSION_MINOR) return false;

        auto sectorCount = ReadValue<uint32_t>(file);
        if (!sectorCount) return false;

        outSectors.resize(*sectorCount);
        for (EditorSector& sector : outSectors)
        {
            auto readSector = ReadSector(file);
            if (!readSector) return false;
            sector = std::move(*readSector);
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
