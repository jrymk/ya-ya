#include "map.h"
#include "model.h"
#include "camera.h"
#include "timer.h"

coord CollideBox::collide(const CollideBox &rhs, coord myOffset, coord rhsOffset) const { // I hope this works because this is such a pile of shit code
    coord correctionVec(0, 0);
    coord myCenter = myOffset + center;
    coord rhsCenter = rhsOffset + rhs.center + coord::getRandCoord();
    if (!isCircle && !rhs.isCircle) {
        if (!(rhsCenter.y - rhs.size.y / 2. > myCenter.y + size.y / 2. || rhsCenter.y + rhs.size.y / 2. < myCenter.y - size.y / 2.)) {
            if ((rhsCenter.x > myCenter.x) && (rhsCenter.x - rhs.size.x / 2.) < (myCenter.x + size.x / 2.))
                correctionVec.x += (rhsCenter.x - rhs.size.x / 2.) - (myCenter.x + size.x / 2.);
            if ((rhsCenter.x < myCenter.x) && (rhsCenter.x + rhs.size.x / 2.) > (myCenter.x - size.x / 2.))
                correctionVec.x += (rhsCenter.x + rhs.size.x / 2.) - (myCenter.x - size.x / 2.);
        }
        if (!(rhsCenter.x - rhs.size.x / 2. > myCenter.x + size.x / 2. || rhsCenter.x + rhs.size.x / 2. < myCenter.x - size.x / 2.)) {
            if ((rhsCenter.y > myCenter.y) && (rhsCenter.y - rhs.size.y / 2.) < (myCenter.y + size.y / 2.))
                correctionVec.y += (rhsCenter.y - rhs.size.y / 2.) - (myCenter.y + size.y / 2.);
            if ((rhsCenter.y < myCenter.y) && (rhsCenter.y + rhs.size.y / 2.) > (myCenter.y - size.y / 2.))
                correctionVec.y += (rhsCenter.y + rhs.size.y / 2.) - (myCenter.y - size.y / 2.);
        }
        // corner tolerance (not perfect) (not much rect-rect collision either so it's prob fine)
        if (std::abs(correctionVec.x) > .1)
            correctionVec.x = 0.;
        if (std::abs(correctionVec.y) > .1)
            correctionVec.y = 0.;
        return correctionVec;
    }
    if (isCircle && rhs.isCircle) {
        if ((rhsCenter - myCenter).len() < size.x / 2. + rhs.size.x / 2.) // we assume size.x == size.y for your lovely circles
            correctionVec = correctionVec + (rhsCenter - myCenter).unit() * ((rhsCenter - myCenter).len() - size.x / 2. - rhs.size.x / 2.);
        return correctionVec;
    }
    coord r1, r2, c;
    double radius;
    if (isCircle && !rhs.isCircle) {
        c = myCenter;
        radius = size.x / 2;
        r1 = rhsCenter - rhs.size / 2; // bottom left
        r2 = rhsCenter + rhs.size / 2; // top right
    }
    if (!isCircle && rhs.isCircle) {
        c = rhsCenter;
        radius = rhs.size.x / 2;
        r1 = myCenter - size / 2;
        r2 = myCenter + size / 2;
    }
    if (c.x > r1.x && c.x < r2.x) { // tangent on horizontal edge
        if (c.y < r1.y && c.y + radius > r1.y) // tangent on bottom edge
            correctionVec.y += r1.y - radius - c.y;
        if (c.y > r2.y && c.y - radius < r2.y) // tangent on top edge
            correctionVec.y += r2.y + radius - c.y;
    }
    if (c.y > r1.y && c.y < r2.y) { // tangent on vertical edge
        if (c.x < r1.x && c.x + radius > r1.x) // tangent on left edge
            correctionVec.x += r1.x - radius - c.x;
        if (c.x > r2.x && c.x - radius < r2.x) // tangent on right edge
            correctionVec.x += r2.x + radius - c.x;
    }
    if (c.x <= r1.x && c.y <= r1.y && (coord(r1.x, r1.y) - c).len() < radius) // tangent on bottom left corner
        correctionVec = correctionVec + (coord(r1.x, r1.y) - c).unit() * ((coord(r1.x, r1.y) - c).len() - radius);
    if (c.x >= r2.x && c.y <= r1.y && (coord(r2.x, r1.y) - c).len() < radius) // tangent on bottom right corner
        correctionVec = correctionVec + (coord(r2.x, r1.y) - c).unit() * ((coord(r2.x, r1.y) - c).len() - radius);
    if (c.x <= r1.x && c.y >= r2.y && (coord(r1.x, r2.y) - c).len() < radius) // tangent on top left corner
        correctionVec = correctionVec + (coord(r1.x, r2.y) - c).unit() * ((coord(r1.x, r2.y) - c).len() - radius);
    if (c.x >= r2.x && c.y >= r2.y && (coord(r2.x, r2.y) - c).len() < radius) // tangent on top right corner
        correctionVec = correctionVec + (coord(r2.x, r2.y) - c).unit() * ((coord(r2.x, r2.y) - c).len() - radius);
    if (!isCircle && rhs.isCircle)
        correctionVec = coord() - correctionVec;
    return correctionVec;
}

void Map::Tile::pushQuads() {
    switch (tileType) {
        case GRASS:
            _pushQuads(&modelGrass[seed % 9]);
            break;
        case DIRT:
            _pushQuads(&modelDirt);
            break;
        case MOAI:
            _pushQuads(&modelGrass[seed % 9]);
            _pushQuads(&modelMoai);
            break;
    }
}

void Map::Tile::setCoord(int x, int y) {
    this->x = x;
    this->y = y;
}

void Map::Tile::neighborUpdate() { // for adapting to neighbor tiles and so on
    switch (tileType) {
        case GRASS:
        case DIRT:
            collideBoxes = {};
            break;
        case MOAI:
            collideBoxes = {CollideBox({.5, .5}, {.6, .6}, true)};
            if (map->exists(x - 1, y) && map->getTile(x - 1, y).tileType == MOAI)
                collideBoxes.push_back(CollideBox({.1, .5}, {.2, .4}, false));
            if (map->exists(x + 1, y) && map->getTile(x + 1, y).tileType == MOAI)
                collideBoxes.push_back(CollideBox({.9, .5}, {.2, .4}, false));
            if (map->exists(x, y - 1) && map->getTile(x, y - 1).tileType == MOAI)
                collideBoxes.push_back(CollideBox({.5, .1}, {.4, .2}, false));
            if (map->exists(x, y + 1) && map->getTile(x, y + 1).tileType == MOAI)
                collideBoxes.push_back(CollideBox({.5, .9}, {.4, .2}, false));
            break;
        case TRUCK:
            collideBoxes = {CollideBox({.5, .5}, {.6, .6}, false)};
            if (map->exists(x - 1, y) && map->getTile(x - 1, y).tileType == TRUCK)
                collideBoxes.push_back(CollideBox({.1, .5}, {.2, .6}, false));
            if (map->exists(x + 1, y) && map->getTile(x + 1, y).tileType == TRUCK)
                collideBoxes.push_back(CollideBox({.9, .5}, {.2, .6}, false));
            if (map->exists(x, y - 1) && map->getTile(x, y - 1).tileType == TRUCK)
                collideBoxes.push_back(CollideBox({.5, .1}, {.6, .2}, false));
            if (map->exists(x, y + 1) && map->getTile(x, y + 1).tileType == TRUCK)
                collideBoxes.push_back(CollideBox({.5, .9}, {.6, .2}, false));
            break;
    }
}

void Map::Tile::setMap(Map* map) {
    this->map = map;
}

void Map::Tile::setTileType(Map::Tile::TileType type) {
    tileType = type;
    map->getTile(x - 1, y).neighborUpdate();
    map->getTile(x + 1, y).neighborUpdate();
    map->getTile(x, y - 1).neighborUpdate();
    map->getTile(x, y + 1).neighborUpdate();
    map->getTile(x - 1, y - 1).neighborUpdate();
    map->getTile(x + 1, y - 1).neighborUpdate();
    map->getTile(x - 1, y + 1).neighborUpdate();
    map->getTile(x + 1, y + 1).neighborUpdate();
    map->getTile(x, y).neighborUpdate(); // chain update is not allowed (only neighbors, just for those simple fences and shit)
}

Map::Tile::Tile() {
    seed = getRandInt();
}

void Map::Tile::_pushQuads(std::vector<Graphics::Quad> const* model, double zDepthOffset) {
    for (auto quad: *model) {
        quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) + coord(x, y));
        quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) + coord(x, y));
        quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) + coord(x, y));
        quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) + coord(x, y));
        quad.zDepth += zDepthOffset + (Camera::getScreenPos(coord(float(x) + .5, float(y) + .5)).y / Camera::getViewport().size.y - 0.5) / 100.;
        // add (.5, .5) because map tiles have origins different from entities

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
