#ifndef YAYA_MAP_H
#define YAYA_MAP_H

#include <vector>
#include <map>
#include "camera.h"

class Map {
public:
    struct CollideBox {
        coord center;
        coord size;
        bool isCircle = false;
    };

    class Tile {
        Map* map = nullptr;

    public:
        int x = 0;
        int y = 0;

        void setCoord(int x, int y);

        void setMap(Map* map);

        enum TileType {
            GRASS,
            DIRT,
            STONE,
        } tileType = GRASS;

        void pushQuads();

        void update();
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