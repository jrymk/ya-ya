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

class Game {
public:
    NeighborsFinder neighborsFinder;
    Controls controls;

    bool showActionList = false;

    std::priority_queue<Action> actionList;
    double updateTime;
    std::map<std::string, std::shared_ptr<Entity>> entities;
    std::shared_ptr<Entity> player;

    Map map;

    Game();

    void update();

    void mapUpdate();

    void setPlayer(std::shared_ptr<Entity> &player);

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

    constexpr static auto properties = std::make_tuple(
            SaveUtilities::property(&Game::entities, "Gm.et"),
            SaveUtilities::property(&Game::player, "Gm.pl")
    );
};

#endif