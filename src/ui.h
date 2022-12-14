#pragma once
#ifndef YAYA_UI_H
#define YAYA_UI_H

#include "graphics.h"

class Game;

class UserInterface {
public:
    Game* game;

    float displayScaling = 1.;

//    enum ControlsDisplay {
//        PICK_UP_ITEM,
//        DROP_ITEM,
//        STORE_ITEM_TO_FACING_CONTAINER,
//    };

    inline UserInterface(Game* game) : game(game) {}

    void renderUI();

    void renderOverlay();

    bool drawButton(const std::wstring &str, UIRect buttonRect, sf::Color bgColor, sf::Color textColor);
};

#endif //YAYA_UI_H
