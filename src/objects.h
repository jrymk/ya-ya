#pragma once
#ifndef _OBJECTS_HPP_
#define _OBJECTS_HPP_

#include "entity.h"

class Egg : public Entity {
public:
    void runAction(Action& action, std::vector<Action>& followUpActions) override;

    void initModel() override;

    Egg();

    void customUpdate() override;

    std::string getDescriptionStr() override;
};

#endif