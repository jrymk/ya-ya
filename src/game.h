#pragma once
#ifndef YAYA_GAME_H
#define YAYA_GAME_H

#include <string>
#include <fstream>
#include <map>
#include <set>
#include <queue>
#include <memory>
#include "entity.h"
#include "saveUtilities.h"
#include "controls.h"
#include "neighborsfinder.h"
#include "map.h"
#include "ui.h"
#include "gamecontroller.h"

class Player;
class Truck;

class Game {
public:
    GameController controller;
    NeighborsFinder neighborsFinder;
    Controls controls;
    UserInterface ui;

    bool showActionList = false;
    bool showCollisionBoxes = false;

    std::priority_queue<Action> actionList;
    std::vector<Action> actionSaveList;  // for game save
    double updateTime;
    std::map<std::string, std::shared_ptr<Entity>> entities;
    std::shared_ptr<Player> player;
    std::shared_ptr<Truck> truck;

    Map map;

    Game();

    void update();

    void mapUpdate();

    void setPlayer(std::shared_ptr<Player> &player);
    void setTruck(std::shared_ptr<Truck> &truck);
    
    void addRandomDuck();

    void processCollisions();

    void runActions();

    virtual void runAction(Action &action, std::vector<Action> &followUpActions);

    std::shared_ptr<Entity> findEntity(std::string id);

    void pushAction(const Action &action);

    std::string newId(EntityType type);

    template<class T>
    inline std::shared_ptr<Entity> &insertEntity(std::shared_ptr<T> &childPtr, bool noInit = false) {
        if (childPtr->id == "undefined") {
            debug << "Entity insertion failed because name is undefined\n";
            return entities.end()->second;
        }
        if (entities.find(childPtr->id) == entities.end()) {
            auto ret = entities.insert({childPtr->id, childPtr});
            if (!noInit)
                pushAction(Action(ret.first->second, Timer::getNow(), ON_CREATION));
            return ret.first->second;
        } else {
            debug << "Entity insertion failed because name \"" << childPtr->id << "\" is already taken\n";
            return entities.end()->second;
        }
    }

    void destroyEntity(std::string id);

    void render();

    void renderMap();

    static constexpr const char* defaultFilePath = ".\\save.ya";

    void save();

    void load(const char* filepath = defaultFilePath);

    void initTruckCollisionBoxes(int baseX, int baseY);

    constexpr static auto properties = std::make_tuple(
            SaveUtilities::property(&Game::controller, "Gm.ctr"),
            SaveUtilities::property(&Game::entities, "Gm.et"),
            SaveUtilities::property(&Game::player, "Gm.pl"),
            SaveUtilities::property(&Game::truck, "Gm.tk"),
            SaveUtilities::property(&Game::actionSaveList, "Gm.ac")
    );
};

#endif