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
    std::vector<std::string> inventory;
    
    Controls(std::map<std::string, Entity*>* entities, NeighborsFinder* neighborsFinder);

    Entity* getFacingEntity(Entity* player, const std::string& filter = "");

    void handleKeyPress(enum sf::Keyboard::Key key);
};

#endif