#pragma once
#ifndef _EGG_H_
#define _EGG_H_

#include "entity.h"

class Egg : public Entity {
public:
    bool genderIsMale = true;
    bool fertilized = false;

    void runAction(Action& action, std::vector<Action>& followUpActions) override;

    void initModel() override;

    Egg();

    void customUpdate() override;

    std::string getDescriptionStr() override;
};

#endif