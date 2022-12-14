#ifndef YAYA_EGGCARTON_H
#define YAYA_EGGCARTON_H

#include "entity.h"
#include "game.h"

class EggCarton : public Entity {
public:
    void runAction(Action &action, std::vector<Action> &followUpActions) override;

    void loadModel() override;

    Game* game;

    EggCarton();

    EggCarton(Game* game);

    void objInit() override;

    enum InventorySlots {
        EGG_0,
        EGG_1,
        EGG_2,
        EGG_3,
        EGG_4,
        EGG_5,
        EGG_6,
        EGG_7,
        EGG_8,
        EGG_9,
    };

    void customUpdate() override;

    void setInventoryProps() override;

    std::string getDescriptionStr() override;

    constexpr static auto properties = std::make_tuple(  /// TODO: set game*
            SaveUtilities::property(&EggCarton::id, "Ec.id"),
            SaveUtilities::property(&EggCarton::type, "Ec.tp"),
            SaveUtilities::property(&EggCarton::position, "Ec.ps"),
            SaveUtilities::property(&EggCarton::deleted, "Ec.dl"),
            SaveUtilities::property(&EggCarton::opacity, "Ec.op")
    );
};


#endif //YAYA_EGGCARTON_H
