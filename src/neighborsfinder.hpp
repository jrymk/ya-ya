#ifndef _NEIGHBORSFINDER_HPP_
#define _NEIGHBORSFINDER_HPP_

#include <map>
#include <set>
#include "entity.hpp"

class NeighborsFinder {
    std::map<std::string, Entity*>* entities;
    std::map<std::pair<int, int>, std::set<std::string>> chunkMembers[5];
    
public:
    std::pair<int, int> getChunk(coord c, int level) {
        return std::make_pair(int(floor(c.x)) & (~0 << level), int(floor(c.y)) & (~0 << level));
    }
    std::pair<int, int> getChunk(std::pair<int, int> p, int level) {
        return std::make_pair(p.first & (~0 << level), p.second & (~0 << level));
    }

    void update() {
        for (auto e : *entities) {
            if (e.second->neighborsFinderMyTile == std::make_pair((int)-1e8, (int)1e8)) {
                e.second->neighborsFinderMyTile = getChunk(e.second->position, 0);
                for (int l = 0; l < 5; l++)
                    chunkMembers[l][getChunk(e.second->position, l)].insert(e.first);
            }
            else {
                for (int l = 0; l < 5; l++) {
                    if (e.second->neighborsFinderMyTile != getChunk(e.second->position, l)) {
                        chunkMembers[l][getChunk(e.second->neighborsFinderMyTile, l)].erase(e.second->id);
                        chunkMembers[l][getChunk(e.second->position, l)].insert(e.second->id);
                        if (l == 0)
                            e.second->neighborsFinderMyTile = getChunk(e.second->position, 0);
                    } 
                    else
                        break;
                }
            }
        }
    }

    void destroyEntry(Entity* e) {
        for (int l = 0; l < 5; l++)
            chunkMembers[l][getChunk(e->neighborsFinderMyTile, l)].erase(e->id);
    }

    std::vector<Entity*> findNeighbors(coord center, double radius, std::string filter = "") {
        std::vector<Entity*> neighbors;
        int d = int(ceil(radius));
        int l = 0;
        for (; l < 4; l++) {
            if (d <= (1 << l))
                break;
        }
        for (int x = ((int(floor(center.x - radius)) - (1 << l)) & (~0 << l)); x <= ((int(ceil(center.x + radius)) + (1 << l)) & (~0 << l)); x+=(1 << l)) {
            for (int y = ((int(floor(center.y - radius)) - (1 << l)) & (~0 << l)); y <= ((int(ceil(center.y + radius)) + (1 << l)) & (~0 << l)); y+=(1 << l)) {
                if (chunkMembers[l].find(std::make_pair(x, y)) == chunkMembers[l].end())
                    continue;
                for (auto& id : chunkMembers[l][std::make_pair(x, y)]) {
                    Entity* e;
                    auto result = entities->find(id);
                    if (result == entities->end()) {
                        debug << "Find entity failed when tring to find \"" << id << "\"\n";
                        e = nullptr;
                    }
                    e = result->second;
                    if (e) {
                        if (filter != "" && e->type != filter)
                            continue;
                        if (e->position.len(center) <= radius)
                            neighbors.push_back(e);
                    }
                }
            }
        }
        return neighbors;
    }

    NeighborsFinder(std::map<std::string, Entity*>* entities): entities(entities) {}
};

#endif