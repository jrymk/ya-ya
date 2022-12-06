#pragma once
#ifndef _CONTROLS_HPP_
#define _CONTROLS_HPP_

#include <map>
#include "entity.h"
#include "camera.h"
#include "neighborsfinder.h"

class Controls {
    std::map<std::string, Entity*>* entities;
    NeighborsFinder* neighborsFinder;

public:
    Controls(std::map<std::string, Entity*>* entities, NeighborsFinder* neighborsFinder);

    Entity* getFacingEntity(Entity* player, EntityType filter = ENTITY);

    void handleKeyPress(enum sf::Keyboard::Key key);
};

#endif