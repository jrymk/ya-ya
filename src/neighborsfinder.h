#pragma once
#ifndef _NEIGHBORSFINDER_H_
#define _NEIGHBORSFINDER_H_

#include <map>
#include <set>
#include "entity.h"

class Game;

class NeighborsFinder {
    Game* game;
    std::map<std::pair<int, int>, std::set<std::shared_ptr<Entity>>> chunkMembers[5];
    
public:
    std::pair<int, int> getChunk(coord c, int level);
    std::pair<int, int> getChunk(std::pair<int, int> p, int level);

    void update();

    void destroyEntry(std::shared_ptr<Entity>& e);

    std::vector<std::shared_ptr<Entity>> findNeighbors(coord center, double radius, EntityType filter = ENTITY);

    NeighborsFinder(Game* game): game(game) {}
};

#endif