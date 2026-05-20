#pragma once

namespace Editor
{
    namespace MapFormat
    {
        inline constexpr char MAGIC[4] = { 'D', 'M', 'A', 'P' };
        inline constexpr uint8_t VERSION_MAJOR = 1;
        inline constexpr uint8_t VERSION_MINOR = 0;

        inline constexpr uint8_t FLAG_CONNECTION = 0x01;
        inline constexpr uint8_t FLAG_PORTAL = 0x02;
    }

    class MapFileIO
    {
    public:
        bool Read(const std::string& filePath, std::vector<EditorSector>& outSectors);
        bool Write(const std::string& filePath, const std::vector<EditorSector>& sectors);

    private:
        std::optional<EditorSector> ReadSector(std::ifstream& file);
        std::optional<EditorWall> ReadWall(std::ifstream& file);

        void WriteSector(std::ofstream& file, const EditorSector& sector);
        void WriteWall(std::ofstream& file, const EditorWall& wall);

        template<typename T>
        static std::optional<T> ReadValue(std::ifstream& file);

        template<typename T>
        static void WriteValue(std::ofstream& file, const T& value);

        static std::optional<Core::Color> ReadColor(std::ifstream& file);
        static void WriteColor(std::ofstream& file, const Core::Color& color);
    };
}
