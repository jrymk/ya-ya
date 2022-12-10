#pragma once
#ifndef _CONTROLS_HPP_
#define _CONTROLS_HPP_

#include <map>
#include "entity.h"
#include "camera.h"
#include "neighborsfinder.h"

class Game;

class Controls {
    Game* game;

public:
    Controls(Game* game);

    std::shared_ptr<Entity> getFacingEntity(std::shared_ptr<Entity> player, EntityType filter = ENTITY);

    void handleKeyPress(enum sf::Keyboard::Key key);
};

#endif