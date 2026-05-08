#pragma once

namespace Core
{
    namespace BSPFormat
    {
        inline constexpr char MAGIC[4] = { 'D', 'B', 'S', 'P' };
        inline constexpr uint8_t VERSION_MAJOR = 1;
        inline constexpr uint8_t VERSION_MINOR = 0;

        // Portal flag bits (single byte per wall)
        inline constexpr uint8_t FLAG_CONNECTION = 0x01;
        inline constexpr uint8_t FLAG_PORTAL = 0x02;
    }

    class BSPFileReader
    {
    public:
        bool Read(const std::string& filePath, BSPMap& outMap);

    private:
        bool ReadSectors(std::ifstream& file, BSPMap& outMap, uint32_t count);
        bool ReadSubSectors(std::ifstream& file, BSPMap& outMap, uint32_t count);
        bool ReadBSPNodes(std::ifstream& file, BSPMap& outMap);

        bool ReadWall(std::ifstream& file, Wall& outWall);

        // Recursively reads a node and all its children; parent is nullptr for root
        std::unique_ptr<BSPNode> ReadNodeRecursive(std::ifstream& file, BSPMap& outMap, BSPNode* parent);

        template<typename T>
        static bool ReadValue(std::ifstream& file, T& value);

        static bool ReadColor(std::ifstream& file, Color& outColor);
        static bool ReadVector2(std::ifstream& file, Vector2& outVec);
        static bool ReadVector2Int(std::ifstream& file, Vector2Int& outVec);
    };
}
