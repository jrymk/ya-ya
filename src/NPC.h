#pragma once
#ifndef YAYA_NPC_H
#define YAYA_NPC_H

#include "entity.h"
#include "game.h"

class npc : public Entity {
public :
    bool genderIsMale = true;
    bool fertilized = false;

    void runAction(Action &action, std::vector<Action> &followUpActions) override;

    void loadModel() override;

    Game* game;

    npc(Game* game);

    void customUpdate() override;

    enum InventorySlots {
        LANSEHYAOJI,
    };

    void setInventoryProps() override;

    std::string getDescriptionStr() override;
};

#endif