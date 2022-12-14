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

    Egg();

    Egg(Game* game);

    void objInit() override;

    enum InventorySlots {
        EMBRYO,
    };

    void customUpdate() override;

    void setInventoryProps() override;

    std::wstring getLocalization(int lang, int strId) override;

    std::string getDescriptionStr() override;

    constexpr static auto properties = std::tuple_cat(Entity::properties, std::make_tuple(
            SaveUtilities::property(&Egg::genderIsMale, "Eg.gd"),
            SaveUtilities::property(&Egg::fertilized, "Eg.fr")
    ));  // reminder: set game*
};

#endif