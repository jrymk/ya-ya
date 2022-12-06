#include "neighborsfinder.h"
#include "game.h"
#include <thread>

std::pair<int, int> NeighborsFinder::getChunk(coord c, int level) {
    return std::make_pair(int(floor(c.x)) & (~0 << level), int(floor(c.y)) & (~0 << level));
}
std::pair<int, int> NeighborsFinder::getChunk(std::pair<int, int> p, int level) {
    return std::make_pair(p.first & (~0 << level), p.second & (~0 << level));
}

void NeighborsFinder::update() {
    for (auto e : game->entities) {
        if (e.second->neighborsFinderMyTile == std::make_pair((int)-1e8, (int)1e8)) {
            e.second->neighborsFinderMyTile = getChunk(e.second->position, 0);
            for (int l = 0; l < 5; l++)
                chunkMembers[l][getChunk(e.second->position, l)].insert(e.second);
        }
        else {
            for (int l = 0; l < 5; l++) {
                if (e.second->neighborsFinderMyTile != getChunk(e.second->position, l)) {
                    chunkMembers[l][getChunk(e.second->neighborsFinderMyTile, l)].erase(e.second);
                    chunkMembers[l][getChunk(e.second->position, l)].insert(e.second);
                    if (l == 0)
                        e.second->neighborsFinderMyTile = getChunk(e.second->position, 0);
                } 
                else
                    break;
            }
        }
    }
}

void NeighborsFinder::destroyEntry(Entity* e) {
    for (int l = 0; l < 5; l++)
        chunkMembers[l][getChunk(e->neighborsFinderMyTile, l)].erase(e);
}

std::vector<Entity*> NeighborsFinder::findNeighbors(coord center, double radius, bool coarse, EntityType filter) {
    std::vector<Entity*> neighbors;
    int d = int(ceil(radius));
    int l = 0;
    for (; l < 2; l++) { // or l < 4
        if (d <= (1 << l))
            break;
    }
    for (int x = ((int(floor(center.x - radius)) - (1 << l)) & (~0 << l)); x <= ((int(ceil(center.x + radius)) + (1 << l)) & (~0 << l)); x+=(1 << l)) {
        for (int y = ((int(floor(center.y - radius)) - (1 << l)) & (~0 << l)); y <= ((int(ceil(center.y + radius)) + (1 << l)) & (~0 << l)); y+=(1 << l)) {
            if (chunkMembers[l].find(std::make_pair(x, y)) == chunkMembers[l].end())
                continue;
            auto chunk = chunkMembers[l][std::make_pair(x, y)];
            for (auto& e : chunk) {
                if (e) {
                    if (filter != ENTITY && e->type != filter)
                        continue;
                    if (coarse || e->position.len(center) <= radius)
                        neighbors.push_back(e);
                }
            }
        }
    }
    return neighbors;
}