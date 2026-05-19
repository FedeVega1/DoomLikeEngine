#pragma once

namespace Editor
{
    struct EditorNode
    {
        Core::Vector2 pos;
        GUID nodeID = Core::NULL_ID_32;
    };

    struct EditorWall
    {
        GUID leftPoint = Core::NULL_ID_32, rightPoint = Core::NULL_ID_32;
        Core::Color topColor, innerColor, bottomColor;
        bool isPortal = false, isConnection = false;
        GUID wallID = Core::NULL_ID_32, portalWallTargetID = Core::NULL_ID_32;
        GUID portalTargetSectorID = Core::NULL_ID_32;
        Core::Vector2 min, max;

        void UpdateBounds(const Core::Vector2& a, const Core::Vector2& b)
        {
            min = Core::Vector2(std::min(a.x, b.x), std::min(a.y, b.y));
            max = Core::Vector2(std::max(a.x, b.x), std::max(a.y, b.y));
        }
    };

    struct EditorSector
    {
        GUID sectorID = Core::NULL_ID_32;
        float floorHeight = 0.f, ceillingHeight = 128.f;
        Core::Color floorColor, ceillingColor;
        std::vector<GUID> walls;
        Core::Vector2 min, max;

        void ResetBounds()
        {
            min = Core::Vector2(FLT_MAX, FLT_MAX);
            max = Core::Vector2(-FLT_MAX, -FLT_MAX);
        }

        void ExpandBounds(const Core::Vector2& point)
        {
            min = Core::Vector2(std::min(min.x, point.x), std::min(min.y, point.y));
            max = Core::Vector2(std::max(max.x, point.x), std::max(max.y, point.y));
        }
    };

    class MapData
    {
    public:
        MapData(GUID initNode, GUID initWall, GUID initSector) : nodes(), walls(), sectors(), nodeCounter(initNode), 
            wallCounter(initWall), sectorCounter(initSector)
        { }

        ~MapData() = default;

        EditorNode& GetNode(GUID id) { return nodes.at(id); }
        EditorWall& GetWall(GUID id) { return walls.at(id); }
        EditorSector& GetSector(GUID id) { return sectors.at(id); }

        const EditorNode& GetNode(GUID id) const { return nodes.at(id); }
        const EditorWall& GetWall(GUID id) const { return walls.at(id); }
        const EditorSector& GetSector(GUID id) const { return sectors.at(id); }

        GUID AddNode(const Core::Vector2& pos);
        GUID AddWall(EditorWall& wall);
        GUID AddSector(EditorSector& sector);

        void ForEachNode(const std::function<void(const EditorNode&)>& callback) const;
        void ForEachWall(const std::function<void(const EditorWall&)>& callback) const;
        void ForEachSector(const std::function<void(const EditorSector&)>& callback) const;

        void RemoveNode(GUID id) { nodes.erase(id); }
        void RemoveWall(GUID id) { walls.erase(id); }
        void RemoveSector(GUID id) { sectors.erase(id); }

    private:
        std::unordered_map<GUID, EditorNode> nodes;
        std::unordered_map<GUID, EditorWall> walls;
        std::unordered_map<GUID, EditorSector> sectors;

        GUID nodeCounter, wallCounter, sectorCounter;
    };

    enum class EditorMode { None, Line, Node, Wall, Sector };

    Core::Sector EditorSectorToSector(const EditorSector& eSector);
    Core::Wall EditorWallToWall(const EditorWall& eWall, const GUID& editorSectorID);
}
