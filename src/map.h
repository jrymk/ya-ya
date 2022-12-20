#ifndef YAYA_MAP_H
#define YAYA_MAP_H

#include <vector>
#include <map>
#include "camera.h"

// not really a reason for this to be here
struct CollideBox {
    coord center;
    coord size;
    bool isCircle = false;

    inline CollideBox() {}

    inline CollideBox(coord center, coord size, bool isCircle = false) : center(center), size(size), isCircle(isCircle) {}

    coord collide(const CollideBox &rhs, coord myOffset, coord rhsOffset) const;

    constexpr static auto properties = std::make_tuple(
            SaveUtilities::property(&CollideBox::center, "Cb.ct"),
            SaveUtilities::property(&CollideBox::size, "CB.sz"),
            SaveUtilities::property(&CollideBox::isCircle, "CB.ic")
    );
};

class Map {
public:
    class Tile {
        Map* map = nullptr;

        void _pushQuads(std::vector<Graphics::Quad> const* model, double zDepthOffset = 0., sf::Color color = sf::Color::White);

    public:
        uint32_t seed;

        int x = 0;
        int y = 0;
        std::vector<CollideBox> collideBoxes;

        void setCoord(int x, int y);

        void setMap(Map* map);

        enum TileType {
            GRASS,
            DIRT,
            MOAI,
            FENCE,
            TRUCK,
        } tileType = GRASS;

        Tile();

        void pushQuads(float zDepthOffset = 0., sf::Color color = sf::Color::White, sf::Color grassColor = sf::Color::White);

        void neighborUpdate();

        void setTileType(TileType type, bool noUpdate = false);

        constexpr static auto properties = std::make_tuple(  // reminder: set map*
                SaveUtilities::property(&Tile::seed, "Tl.sd"),
                SaveUtilities::property(&Tile::x, "Tl.xx"),
                SaveUtilities::property(&Tile::y, "Tl.yy"),
                SaveUtilities::property(&Tile::collideBoxes, "Tl.clb"),
                SaveUtilities::property(&Tile::tileType, "Tl.tp")
        );
    };

    // the map will be divided into 16x16 chunks
    class Chunk {
        Map* map = nullptr;

    public:
        int basex;
        int basey;

        std::vector<Tile> tiles; // yyyyxxxx (x = lsb)

        Chunk() {}

        Chunk(int bx, int by, Map* map);

        void setMap(Map* map);

        constexpr static auto properties = std::make_tuple(  // reminder: set map*
                SaveUtilities::property(&Chunk::basex, "Cn.xx"),
                SaveUtilities::property(&Chunk::basey, "Cn.yy"),
                SaveUtilities::property(&Chunk::tiles, "Cn.tl")
        );
    };

    std::map<std::pair<int, int>, Chunk> tileData;

    bool exists(int x, int y);

    Tile &getTile(int x, int y);

    constexpr static auto properties = std::make_tuple(  // reminder: set map*
            SaveUtilities::property(&Map::tileData, "Mpp.td")
    );
};

#endif