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

    inline CollideBox(coord center, coord size, bool isCircle = false) : center(center), size(size), isCircle(isCircle) {}

    coord collide(const CollideBox &rhs, coord myOffset, coord rhsOffset) const;
};

class Map {
public:
    class Tile {
        Map* map = nullptr;
        uint32_t seed;

    public:
        int x = 0;
        int y = 0;
        std::vector<CollideBox> collideBoxes;

        void setCoord(int x, int y);

        void setMap(Map* map);

        enum TileType {
            GRASS,
            DIRT,
            STONE,
        } tileType = GRASS;

        Tile();

        void pushQuads();

        void neighborUpdate();

        void setTileType(TileType type);
    };

    // the map will be divided into 16x16 chunks
    class Chunk {
        Map* map = nullptr;

    public:
        int basex;
        int basey;

        std::vector<Tile> tiles; // yyyyxxxx (x = lsb)


        Chunk(int bx, int by, Map* map);
    };

    std::map<std::pair<int, int>, Chunk> tileData;

    bool exists(int x, int y);

    Tile &getTile(int x, int y);
};

#endif