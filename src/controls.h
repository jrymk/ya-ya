#pragma once
#ifndef YAYA_CONTROLS_H
#define YAYA_CONTROLS_H

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
    enum ControlsActions {
        UNDEFINED,
        NONE_NO_ITEM_SELECTED,
        PICK_UP_ITEM,
        PICK_UP_CONTAINER,
        DROP_ITEM,
        DROP_CONTAINER,
        NONE_CONTAINER_FULL,
        NONE_CONTAINER_MISMATCH,
        STORE_ITEM,
    };

    ControlsActions leftMouseClickAction;
    ControlsActions leftMouseAltClickAction;
    ControlsActions rightMouseClickAction;
    ControlsActions rightMouseAltClickAction;
    int dirPadPress = 0b0000; // right left down up

    std::shared_ptr<Entity> facingEntity;

    Map::Tile* facingTile = nullptr;


    Controls(Game* game);

    void update();

    void handleKeyPress(sf::Event &event);

    void handleMousePress(sf::Event &event);
};

#endif