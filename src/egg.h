#pragma once
#ifndef YAYA_EGG_H
#define YAYA_EGG_H

#include "entity.h"
#include "game.h"

class Egg : public Entity {
public:
    bool genderIsMale = true;
    bool fertilized = false;

    void runAction(Action &action, std::vector<Action> &followUpActions) override;

    void loadModel() override;

    Game* game;

    Egg(Game* game);

    enum InventorySlots {
        EMBRYO,
    };

    void customUpdate() override;

    void setInventoryProps() override;

    std::string getDescriptionStr() override;

};

#endif