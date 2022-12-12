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

    std::shared_ptr<Entity> getFacingEntity(std::shared_ptr<Entity> player, EntityType filter = ENTITY);

public:
    std::shared_ptr<Entity> facingEntity;

    Controls(Game* game);

    void update();

    void handleKeyPress(sf::Event &event);

    void handleMousePress(sf::Event &event);
};

#endif