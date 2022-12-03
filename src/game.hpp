#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "entity.hpp"
#include "duck.hpp"

class Game {
public:
    std::vector<Entity*> entities;
    std::vector<Duck*> ducks;

    void update() {
        for (auto entity : entities) {
            entity->update();
        }
    }

    void render() {
        for (auto entity : entities) {
            entity->pushQuads();
        }
    }

};

#endif