#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "duck.hpp"

class Game {
public:
    std::vector<Duck*> ducks;

    void update() {
        for (auto duck : ducks) {
            duck->update();
        }
    }

    void render() {
        for (auto duck : ducks) {
            duck->pushQuads();
        }
    }

};

#endif