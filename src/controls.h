#pragma once
#ifndef _CONTROLS_HPP_
#define _CONTROLS_HPP_

#include <map>
#include "entity.h"
#include "camera.h"
#include "neighborsfinder.h"

class Game;

class Controls {
    std::map<std::string, Entity*>* entities;
    Game* game;

public:
    Controls(Game* game);

    Entity* getFacingEntity(Entity* player, EntityType filter = ENTITY);

    void handleKeyPress(enum sf::Keyboard::Key key);
};

#endif