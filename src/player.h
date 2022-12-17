#pragma once
#ifndef YAYA_PLAYER_H
#define YAYA_PLAYER_H

#include "entity.h"
#include "game.h"

class Player : public Entity {
private:

public:
    void loadModel() override;

    void runAction(Action &action, std::vector<Action> &followUpActions) override;

    Player();

    Player(Game* game);

    void objInit();

    Game* game;

    enum InventorySlots {
        LEFT_HAND,
        RIGHT_HAND,
    };

    void setInventoryProps() override;

    void customUpdate() override;

    void environmentUpdate() override;

    constexpr static auto properties = std::tuple_cat(Entity::properties, std::make_tuple(
    ));  // reminder: set game*
};

#endif