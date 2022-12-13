#pragma once
#ifndef _CONTROLS_HPP_
#define _CONTROLS_HPP_

#include <map>
#include "map.h"
#include "entity.h"
#include "camera.h"
#include "neighborsfinder.h"

class Game;

class Controls {
    Game* game;

    std::shared_ptr<Entity> getFacingEntity(EntityType filter = ENTITY);

    Map::Tile &getFacingTile();

public:
    int dirPadPress = 0b0000; // right left down up
    std::shared_ptr<Entity> facingEntity;
    Map::Tile* facingTile = nullptr;

    Controls(Game* game);

    void update();

    void handleKeyPress(sf::Event &event);

    void handleMousePress(sf::Event &event);
};

#endif