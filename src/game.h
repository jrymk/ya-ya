#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include <fstream>
#include <map>
#include <set>
#include "entity.h"
#include "duck.h"
#include "egg.h"
#include "serialization.h"
#include "controls.h"
#include "neighborsfinder.h"

class Game {
public:
    NeighborsFinder neighborsFinder;
    Controls controls;

    bool showActionList = true;

    std::vector<Action> actionList;
    double updateTime;
    std::map<std::string, Duck*> ducks;
    std::map<std::string, Entity*> entities;

    Game();

    void update();

    void processCollisions();

    void runActions();

    virtual void runAction(Action& action, std::vector<Action>& followUpActions);

    Entity* findEntity(std::string id);

    void pushAction(std::string id, Timer timer, std::string action);

    std::string newId(const std::string& type);

    void insertEntity(Entity* entity);

    void destroyEntity(std::string id);

    void render();

    static constexpr const char* defaultFilePath = ".\\save.ya";
    
    void save();

    void load(const char* filepath = defaultFilePath);
};

#endif