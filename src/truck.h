#pragma once
#ifndef YAYA_TRUCK_H
#define YAYA_TRUCK_H

#include "entity.h"
#include "game.h"

class Truck : public Entity {
public:
    void runAction(Action &action, std::vector<Action> &followUpActions) override;

    void loadModel() override;

    Game* game;

    Truck();

    Truck(Game* game);

    void objInit() override;

    enum InventorySlots {
        TRUCK_A0,
        TRUCK_A1,
        TRUCK_A2,
        TRUCK_A3,
        TRUCK_B0,
        TRUCK_B1,
        TRUCK_B2,
        TRUCK_B3,
        TRUCK_C0,
        TRUCK_C1,
        TRUCK_C2,
        TRUCK_C3,
        TRUCK_D0,
        TRUCK_D1,
        TRUCK_D2,
        TRUCK_D3,
    };

    void customUpdate() override;

    void setInventoryProps() override;

    std::wstring getLocalization(int lang, int strId) override;

    std::string getDescriptionStr() override;

    constexpr static auto properties = std::tuple_cat(Entity::properties, std::make_tuple(
    ));  // reminder: set game*
};


#endif //YAYA_TRUCK_H
