#pragma once
#ifndef YAYA_GAMECONTROLLER_H
#define YAYA_GAMECONTROLLER_H

#include "timer.h"

#define DAY_LENGTH 90.
#define DAY_START_CUTSCENE_LENGTH 2.
#define DAY_END_CUTSCENE_LENGTH 0. // TODO: end screen

class Game;

class GameController {
    Game* game;

public:
    // ui will read from this
    enum GameState {
        TITLE_SCREEN,
        PAUSE_SCREEN,
        DAY_START_SCENE,
        GAMEPLAY,
        DAY_END_SCENE,
    } gameState = TITLE_SCREEN;

    enum GameActions {
        BTN_START_NEW_GAME,
        BTN_START_LOAD_GAME,

    };

    double cash = 0.;
    int dayCount = 0; // 1 is first day. no question asked
    Timer startOfDayTp;

public:
    GameController(Game* game);

    void setGame(Game* game);

    void handleAction(GameActions action);

    void update();

    void startDaySequence();

    void endDaySequence();

    void loadToSaveFile();

    void saveToSaveFile();

    void loadTestWorld();

    constexpr static auto properties = std::make_tuple(  // remember to set game*
            SaveUtilities::property(&GameController::cash, "Gc.ca"),
            SaveUtilities::property(&GameController::dayCount, "Gc.dc"),
            SaveUtilities::property(&GameController::startOfDayTp, "Gc.stp")
    );
};

#endif //YAYA_GAMECONTROLLER_H
