#pragma once
#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "entity.h"

class Player: public Entity {
private:

public:
    void initModel() override;

    Player();

    void customUpdate() override;

};

#endif