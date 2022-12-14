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

void Map::Tile::pushQuads(float zDepthOffset, sf::Color color, sf::Color grassColor) {
    std::vector<Graphics::Quad> grassSubTile;
    grassSubTile.push_back(modelGrass);
    UIVec xDelta = UIVec(grassSubTile[0].t2 - grassSubTile[0].t3) / 10.;
    UIVec yDelta = UIVec(grassSubTile[0].t0 - grassSubTile[0].t3) / 10.;
    grassSubTile[0].t0 = (UIVec(grassSubTile[0].t3) + xDelta * ((seed / 10) % 10 + 0) + yDelta * (seed % 10 + 1)).getVec2f();
    grassSubTile[0].t1 = (UIVec(grassSubTile[0].t3) + xDelta * ((seed / 10) % 10 + 1) + yDelta * (seed % 10 + 1)).getVec2f();
    grassSubTile[0].t2 = (UIVec(grassSubTile[0].t3) + xDelta * ((seed / 10) % 10 + 1) + yDelta * (seed % 10 + 0)).getVec2f();
    grassSubTile[0].t3 = (UIVec(grassSubTile[0].t3) + xDelta * ((seed / 10) % 10 + 0) + yDelta * (seed % 10 + 0)).getVec2f();

    switch (tileType) {
        case GRASS:
            _pushQuads(&grassSubTile, zDepthOffset, grassColor);
            break;
        case DIRT:
            _pushQuads(&modelDirt, zDepthOffset, grassColor);
            break;
        case MOAI:
            _pushQuads(&grassSubTile, zDepthOffset, grassColor);
            _pushQuads(&modelMoai, zDepthOffset, color);
            break;
        case FENCE: {
            _pushQuads(&grassSubTile, zDepthOffset, grassColor);
            std::vector<Graphics::Quad> fenceSubTile;
            fenceSubTile.push_back(modelFence[0]);
            if (map->exists(x + 1, y) && map->getTile(x + 1, y).tileType == FENCE)
                fenceSubTile.push_back(modelFence[1]);
            if (map->exists(x - 1, y) && map->getTile(x - 1, y).tileType == FENCE)
                fenceSubTile.push_back(modelFence[2]);
            if (map->exists(x, y - 1) && map->getTile(x, y - 1).tileType == FENCE)
                fenceSubTile.push_back(modelFence[3]);
            if (map->exists(x, y + 1) && map->getTile(x, y + 1).tileType == FENCE)
                fenceSubTile.push_back(modelFence[4]);

            _pushQuads(&fenceSubTile, zDepthOffset, color);
            break;
        }
        case TRUCK:
            _pushQuads(&grassSubTile, zDepthOffset, color);
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
            break;
        case FENCE:
            collideBoxes = {CollideBox({.5, .5}, {.2, .2}, false)};
            if (map->exists(x - 1, y) && map->getTile(x - 1, y).tileType == FENCE)
                collideBoxes.push_back(CollideBox({.2, .5}, {.4, .13}, false));
            if (map->exists(x + 1, y) && map->getTile(x + 1, y).tileType == FENCE)
                collideBoxes.push_back(CollideBox({.8, .5}, {.4, .13}, false));
            if (map->exists(x, y - 1) && map->getTile(x, y - 1).tileType == FENCE)
                collideBoxes.push_back(CollideBox({.5, .2}, {.13, .4}, false));
            if (map->exists(x, y + 1) && map->getTile(x, y + 1).tileType == FENCE)
                collideBoxes.push_back(CollideBox({.5, .8}, {.13, .4}, false));
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

void Map::Tile::setTileType(Map::Tile::TileType type, bool noUpdate) {
    tileType = type;
    if (noUpdate)
        return;
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

void Map::Tile::_pushQuads(std::vector<Graphics::Quad> const* model, double zDepthOffset, sf::Color color) {
    for (auto quad: *model) {
        quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) + coord(x, y));
        quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) + coord(x, y));
        quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) + coord(x, y));
        quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) + coord(x, y));
        quad.zDepth += zDepthOffset + (Camera::getScreenPos(coord(float(x) + .5, float(y) + .5)).y / Camera::getViewport().size.y - 0.5) / 100.;
        // add (.5, .5) because map tiles have origins different from entities

        quad.c0 = color;
        quad.c1 = color;
        quad.c2 = color;
        quad.c3 = color;

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

void Map::Chunk::setMap(Map* map) {
    this->map = map;
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
