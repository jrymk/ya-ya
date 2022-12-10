#pragma once
#ifndef _DUCK_H_
#define _DUCK_H_

#include "entity.h"

class Duck : public Entity {
public:
    bool genderIsMale = true;
    bool fertilized = false; // will the next egg be fertilized, set after having sex

    void runAction(Action &action, std::vector<Action> &followUpActions) override;

    void initModel() override;

    Duck();

    void customUpdate() override;

    std::string getDescriptionStr() override;

    constexpr static auto properties = std::make_tuple(
            SaveUtilities::property(&Duck::position, "Dk.ps")
    );
};

#endif