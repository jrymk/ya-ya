#pragma once
#ifndef YAYA_UI_H
#define YAYA_UI_H

#include "graphics.h"

class Game;

class UserInterface {
public:
    Game* game;

    inline UserInterface(Game* game) : game(game) {}

    void renderUI();


};

#endif //YAYA_UI_H
