#ifndef YAYA_MAP_H
#define YAYA_MAP_H

#include <vector>
#include <map>

class Map {
public:
    class Tile {
    public:
        enum TileType {
            GRASS,

        } tileType;

        void pushQuads(int x, int y);
    };

    // the map will be divided into 16x16 chunks
    class Chunk {
    public:
        std::vector<Tile> tiles; // yyyyxxxx (x = lsb)

        Chunk();
    };

    std::map<std::pair<int, int>, Chunk> mapData;


    Tile &getTile(int x, int y);
};

#endif