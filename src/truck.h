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
        TRUCK_0,
        TRUCK_1,
        TRUCK_2,
        TRUCK_3,
        TRUCK_4,
        TRUCK_5,
        TRUCK_6,
        TRUCK_7,
        TRUCK_8,
        TRUCK_9,
    };

    void customUpdate() override;

    void setInventoryProps() override;

    std::wstring getLocalization(int lang, int strId) override;

    std::string getDescriptionStr() override;

    constexpr static auto properties = std::tuple_cat(Entity::properties, std::make_tuple(
    ));  // reminder: set game*
};


#endif //YAYA_TRUCK_H
