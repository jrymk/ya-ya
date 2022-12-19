#pragma once
#ifndef YAYA_CONTROLS_H
#define YAYA_CONTROLS_H

#include <map>
#include "map.h"
#include "entity.h"
#include "camera.h"
#include "neighborsfinder.h"
#include "audio.h"

class Game;

class Controls {
    Game* game;

    std::shared_ptr<Entity> getFacingEntity(EntityType filter = ENTITY);

    Map::Tile &getFacingTile();

    int tryStoreToContainer(const std::shared_ptr<Entity> &container, const std::shared_ptr<Entity> &item,
                            bool useRandomSlot = false); // returns the slot, or -1 if illegal, -2 if full
    int tryPickUpFromContainer(const std::shared_ptr<Entity> &container);

    void changeOwner(const std::shared_ptr<Entity> &item, const std::shared_ptr<Entity> &newContainer = nullptr, int newSlot = -1);

public:
    enum ControlsActions { // unless specified, subject would be the one in the player's hand
        UNDEFINED,
        NONE_NO_ITEM_SELECTED,
        PICK_UP_ITEM,
        PICK_UP_CONTAINER,
        PICK_UP_ITEM_FROM_FACING_CONTAINER,
        DROP_ITEM,
        DROP_CONTAINER,
        NONE_CONTAINER_FULL,
        NONE_CONTAINER_MISMATCH,
        STORE_ITEM_TO_FACING_CONTAINER,
        STORE_ITEM_TO_OTHER_HAND_CONTAINER,
        STORE_FACING_ITEM_TO_CONTAINER, // using the other hand, so it has to be empty (for this action the errors will be hidden)
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

    void handleMouseScroll(sf::Event &event);

    void handleSoundOnAction(sf::Event &event, Audio &audio);
};

#endif