#pragma once

namespace Editor
{
    struct CompilerResult
    {
        std::vector<Core::Sector> sectors;
        std::vector<Core::SubSector> subSectors;
        std::unique_ptr<Core::BSPNode> rootNode;
        bool success = false;
    };

    class BSPCompiler
    {
    public:
        bool Compile(const std::vector<EditorSector>& editorSectors, const std::string& outputPath);

    private:
        CompilerResult BuildBSP(const std::vector<EditorSector>& editorSectors);

        std::unique_ptr<Core::BSPNode> GenerateLeafs(std::vector<Core::Wall> walls, Core::BSPNode* parentNode, uint8_t childFlag);

        Core::Splitter SelectSplitter(const std::vector<Core::Wall>& walls, const Core::BoundingBox& bounds) const;
        Core::BoundingBox CalculateBounds(const std::vector<Core::Wall>& walls) const;

        // Cross-product-based wall partition: fills frontWalls and backWalls
        void PartitionWalls(const std::vector<Core::Wall>& walls, const Core::Splitter& splitter, std::vector<Core::Wall>& outFront,
            std::vector<Core::Wall>& outBack) const;

        // Splits a wall at the splitter plane; returns false if no intersection
        bool SplitWall(const Core::Wall& wall, const Core::Splitter& splitter, Core::Wall& outFront, Core::Wall& outBack) const;
        bool IsConvex(const std::vector<Core::Wall>& walls) const;
        std::vector<Core::Vector2> BuildPointChain(const std::vector<Core::Wall>& walls) const;

        static bool IsInverseWall(const Core::Wall& a, const Core::Wall& b);

        // Simulate how many walls would be on front/back/split for a given splitter
        void SimulateSplits(const std::vector<Core::Wall>& walls, const Core::Splitter& splitter, int& outFront, int& outBack,
            int& outSplit) const;

        bool WriteResult(const CompilerResult& result, const std::string& outputPath) const;
        void WriteNodeRecursive(std::ofstream& file, const Core::BSPNode* node) const;

        template<typename T>
        static void WriteValue(std::ofstream& file, const T& value);

        static void WriteColor(std::ofstream& file, const Core::Color& color);
        static void WriteVector2(std::ofstream& file, const Core::Vector2& v);
        static void WriteVector2Int(std::ofstream& file, const Core::Vector2Int& v);

        std::vector<Core::SubSector> compiledSubSectors;
        uint32_t nextNodeID = 0;
        uint32_t nextSubSectorID = 0;

        static const Core::Vector2Int DIRS[6];
    };
}
