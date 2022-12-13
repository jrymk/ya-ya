#ifndef YAYA_MAP_H
#define YAYA_MAP_H

#include <vector>
#include <map>

class Map {
public:
    class Tile {
    public:
        int x = 0;
        int y = 0;

        void setCoord(int x, int y);

        enum TileType {
            GRASS,

        } tileType;

        void pushQuads();
    };

    // the map will be divided into 16x16 chunks
    class Chunk {
    public:
        int basex;
        int basey;

        std::vector<Tile> tiles; // yyyyxxxx (x = lsb)

        Chunk(int bx, int by);
    };

    std::map<std::pair<int, int>, Chunk> mapData;


    Tile &getTile(int x, int y);
};

#endif