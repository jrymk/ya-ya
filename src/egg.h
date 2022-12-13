#pragma once
#ifndef _EGG_H_
#define _EGG_H_

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