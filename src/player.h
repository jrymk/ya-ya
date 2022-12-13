#pragma once
#ifndef YAYA_PLAYER_H
#define YAYA_PLAYER_H

#include "entity.h"
#include "game.h"

class Player : public Entity {
private:

public:
    void loadModel() override;

    Player(Game* game);

    Player(Game* game, std::shared_ptr<Entity> &entity);

    Game* game;

    enum InventorySlots {
        LEFT_HAND,
        RIGHT_HAND,
    };

    void setInventoryProps() override;

    void customUpdate() override;

    void environmentUpdate() override;

    constexpr static auto properties = std::make_tuple(  // reminder: update constructor
            SaveUtilities::property(&Player::id, "Pl.id"),
            SaveUtilities::property(&Player::type, "Pl.tp"),
            SaveUtilities::property(&Player::position, "Pl.ps"),
            SaveUtilities::property(&Player::deleted, "Pl.dl")
    );
};

#endif