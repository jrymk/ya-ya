#pragma once
#ifndef YAYA_DUCK_H
#define YAYA_DUCK_H

#include "entity.h"
#include "audio.h"

class Duck : public Entity {
public:
    enum GrowStage {
        DUCKLING, CHILD, GROWN
    } growStage;
    
    bool genderIsMale = true;
    bool fertilized = false; // will the next egg be fertilized, set after having sex

    void runAction(Action &action, std::vector<Action> &followUpActions) override;

    void loadModel() override;

    std::vector<Graphics::Quad> duckModel;

    Game* game;

    Audio duckAudio;

    Duck();

    Duck(Game* game);

    void objInit() override;

    enum InventorySlots {
        EGG_0,
        EGG_1,
        BEAK,
    };

    void environmentUpdate() override;

    void customUpdate() override;

    void setInventoryProps() override;

    std::wstring getLocalization(int lang, int strId) override;

    std::string getDescriptionStr() override;

    constexpr static auto properties = std::tuple_cat(Entity::properties, std::make_tuple(
            SaveUtilities::property(&Duck::growStage, "Dk.gsg"),
            SaveUtilities::property(&Duck::genderIsMale, "Dk.gd"),
            SaveUtilities::property(&Duck::fertilized, "Dk.fr")
    ));  // reminder: set game*
};

#endif