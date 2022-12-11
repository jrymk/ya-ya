#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include <fstream>
#include <map>
#include <set>
#include <queue>
#include <memory>
#include "entity.h"
#include "serialization.h"
#include "controls.h"
#include "neighborsfinder.h"

class Game {
public:
    NeighborsFinder neighborsFinder;
    Controls controls;

    bool showActionList = false;

    std::priority_queue<Action> actionList;
    double updateTime;
    std::map<std::string, std::shared_ptr<Entity>> entities;
    std::shared_ptr<Entity> player;

    Game();

    void update();

    void setPlayer(std::shared_ptr<Entity> &player);

    void processCollisions();

    void runActions();

    virtual void runAction(Action &action, std::vector<Action> &followUpActions);

    std::shared_ptr<Entity> findEntity(std::string id);

    void pushAction(const Action &action);

    std::string newId(EntityType type);

    template<class T>
    inline std::shared_ptr<Entity> &insertEntity(std::shared_ptr<T> &childPtr) {
        if (childPtr->id == "undefined") {
            debug << "Entity insertion failed because name is undefined\n";
            return entities.end()->second;
        }
        if (entities.find(childPtr->id) == entities.end()) {
            auto ret = entities.insert({childPtr->id, childPtr});
            pushAction(Action(ret.first->second, Timer::getNow(), INIT));
            return ret.first->second;
        } else {
            debug << "Entity insertion failed because name \"" << childPtr->id << "\" is already taken\n";
            return entities.end()->second;
        }
    }

    void destroyEntity(std::string id);

    void render();

    static constexpr const char* defaultFilePath = ".\\save.ya";

    void save();

    void load(const char* filepath = defaultFilePath);

    constexpr static auto properties = std::make_tuple(
            SaveUtilities::property(&Game::entities, "Gm.et"),
            SaveUtilities::property(&Game::player, "Gm.pl")
    );
};

#endif