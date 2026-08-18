#pragma once

namespace Core
{
    typedef uint32_t GUID;

    inline constexpr uint64_t NULL_ID_64 = 0xFFFFFFFFFFFFFFFFULL;
    inline constexpr GUID NULL_ID_32 = 0xFFFFFFFFU;

    struct Color
    {
        uint8_t r, g, b;

        Color() : r(0), g(0), b(0) {  }
        Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {  }
        bool operator==(const Color& other) const { return r == other.r && g == other.g && b == other.b; }

        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
    };

    inline const Color Color::Black = Color(0, 0, 0);
    inline const Color Color::White = Color(255, 255, 255);
    inline const Color Color::Red = Color(255, 0, 0);
    inline const Color Color::Green = Color(0, 255, 0);
    inline const Color Color::Blue = Color(0, 0, 255);

    struct Wall
    {
        Vector2 leftPoint, rightPoint;
        Color topColor, innerColor, bottomColor;
        bool isPortal = false, isConnection = false;

        uint32_t wallID = NULL_ID_32, portalWallTargetID = NULL_ID_32;
        uint32_t parentSectorID = NULL_ID_32, portalTargetSectorID = NULL_ID_32;

        Wall* portalTargetWall = nullptr;

        Vector2 GetMiddlePoint() const { return { (leftPoint.x + rightPoint.x) * 0.5f, (leftPoint.y + rightPoint.y) * 0.5f }; }
        bool VectorInFront(const Vector2& point) const;
    };

    struct Sector
    {
        uint32_t sectorID = NULL_ID_32;
        float floorHeight = 0.f;
        float ceilingHeight = 128.f;
        Color floorColor;
        Color ceilingColor;
        Vector2 center;
    };

    struct SubSector
    {
        uint32_t subSectorID = NULL_ID_32;
        std::vector<Wall> walls;
    };

    struct BoundingBox
    {
        Vector2Int topPoint;
        Vector2Int bottomPoint;
    };

    struct Splitter
    {
        Vector2Int startPoint;
        Vector2Int segment;
        Vector2 dir;

        bool VectorInFront(const Vector2& point) const;
    };

    struct BSPNode
    {
        static constexpr uint8_t FLAG_ROOT  = 0xFF;
        static constexpr uint8_t FLAG_FRONT = 0xBB;
        static constexpr uint8_t FLAG_BACK  = 0xCC;

        uint8_t childFlag  = FLAG_ROOT;
        uint32_t nodeID = NULL_ID_32;
        Splitter splitter;
        BoundingBox boundingBox;

        SubSector* subSector = nullptr;
        BSPNode* parentNode = nullptr;

        std::unique_ptr<BSPNode> frontNode, backNode;

        bool IsLeafNode() const { return !frontNode && !backNode; }
    };

    struct BSPMap
    {
        std::vector<Sector> sectors;
        std::vector<SubSector> subSectors;
        std::unique_ptr<BSPNode> rootNode;

        void LinkPortals();

        Wall* FindWallByID(uint64_t wallID);
        const Sector* FindSectorByID(uint32_t sectorID) const;
    };
}
