#include "map.h"
#include "model.h"
#include "camera.h"

void Map::Tile::pushQuads() {
    for (auto quad: modelTile) {
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

Map::Chunk::Chunk(int bx, int by) : basex(bx), basey(by) {
    tiles.resize(256);
    for (int i = 0; i < 256; i++)
        tiles[i].setCoord(basex + (i & 0b1111), basey + ((i >> 4) & 0b1111));
}

Map::Tile &Map::getTile(int x, int y) {
    auto c = std::make_pair(x & ~0b1111, y & ~0b1111);
    auto res = mapData.find(c);
    if (res == mapData.end()) {
        mapData.insert(std::make_pair(c, Chunk(c.first, c.second)));
        res = mapData.find(c);
    }
    return res->second.tiles[((y & 0b1111) << 4) + (x & 0b1111)];
}
