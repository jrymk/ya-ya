#include "map.h"
#include "model.h"
#include "camera.h"

void Map::Tile::pushQuads() {
    std::vector<Graphics::Quad> const* model = &modelGrass;

    switch (tileType) {
        case GRASS:
            model = &modelGrass;
            break;
        case DIRT:
            model = &modelDirt;
            break;
        case STONE:
            model = &modelStone;
            break;
    }

    for (auto quad: *model) {
        quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) + coord(x, y));
        quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) + coord(x, y));
        quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) + coord(x, y));
        quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) + coord(x, y));
        quad.zDepth += (Camera::getScreenPos(coord(x, y)).y / Camera::getViewport().size.y - 0.5) / 100.;

        quad.c0 = sf::Color(255, 255, 255, 255);
        quad.c1 = sf::Color(255, 255, 255, 255);
        quad.c2 = sf::Color(255, 255, 255, 255);
        quad.c3 = sf::Color(255, 255, 255, 255);

        Graphics::insertQuad(quad);

        if (Graphics::showWireframe) {
            Graphics::insertUserWireframe(
                    Camera::getScreenPos(coord(x, y)) + UIVec(5., 0.),
                    Camera::getScreenPos(coord(x, y)) + UIVec(0., 5.),
                    Camera::getScreenPos(coord(x, y)) + UIVec(-5., 0.),
                    Camera::getScreenPos(coord(x, y)) + UIVec(0., -5.),
                    sf::Color::Magenta, sf::Color::Black
            );
        }
    }
}

void Map::Tile::setCoord(int x, int y) {
    this->x = x;
    this->y = y;
}

void Map::Tile::neighborUpdate() { // for adapting to neighbor tiles and so on
    switch (tileType) {
//        case STONE:
//            if (!map->exists(x, y + 1))
//                break;
//            Map::Tile &tile = map->getTile(x, y + 1);
//            if (tile.tileType != STONE)
//                tile.tileType = STONE;
//            break;
    }
}

void Map::Tile::setMap(Map* map) {
    this->map = map;
}

void Map::Tile::setTileType(Map::Tile::TileType type) {
    tileType = type;
    switch (tileType) {
        case GRASS:
        case DIRT:
            collideBoxes = {};
            break;
        case STONE:
            collideBoxes = {
                    CollideBox({.5, .5}, {.7, .7}, true)
            };
            break;
    }
    map->getTile(x - 1, y).neighborUpdate();
    map->getTile(x + 1, y).neighborUpdate();
    map->getTile(x, y - 1).neighborUpdate();
    map->getTile(x, y + 1).neighborUpdate();
}

bool Map::exists(int x, int y) {
    return tileData.find(std::make_pair(x & ~0b1111, y & ~0b1111)) != tileData.end();
}

Map::Chunk::Chunk(int bx, int by, Map* map) : basex(bx), basey(by), map(map) {
    tiles.resize(256);
    for (int i = 0; i < 256; i++) {
        tiles[i].setCoord(basex + (i & 0b1111), basey + ((i >> 4) & 0b1111));
        tiles[i].setMap(map);
    }
}


Map::Tile &Map::getTile(int x, int y) {
    auto c = std::make_pair(x & ~0b1111, y & ~0b1111);
    auto res = tileData.find(c);
    if (res == tileData.end()) {
        tileData.insert(std::make_pair(c, Chunk(c.first, c.second, this)));
        res = tileData.find(c);
    }
    return res->second.tiles[((y & 0b1111) << 4) + (x & 0b1111)];
}
