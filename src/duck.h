#pragma once
#ifndef YAYA_DUCK_H
#define YAYA_DUCK_H

#include "entity.h"

class Duck : public Entity {
public:
    bool genderIsMale = true;
    bool fertilized = false; // will the next egg be fertilized, set after having sex

    void runAction(Action &action, std::vector<Action> &followUpActions) override;

    void loadModel() override;

    Game* game;

    Duck(Game* game);

    Duck(Game* game, std::shared_ptr<Entity> &entity);

    enum InventorySlots {
        EGG_0,
        EGG_1,
        BEAK,
    };

    void environmentUpdate() override;

    void customUpdate() override;

    void setInventoryProps() override;

    std::string getDescriptionStr() override;

    constexpr static auto properties = std::make_tuple(  // reminder: update constructor
            SaveUtilities::property(&Duck::id, "Dk.id"),
            SaveUtilities::property(&Duck::type, "Dk.tp"),
            SaveUtilities::property(&Duck::position, "Dk.ps"),
            SaveUtilities::property(&Duck::deleted, "Dk.dl")
    );
};

#endif