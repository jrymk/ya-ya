#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "entity.hpp"
#include "duck.hpp"
#include "objects.hpp"


class Game {
public:
    std::vector<Entity*> entities;
    std::vector<Duck*> ducks;
    std::vector<Entity::Action> globalActions;

    void update() {
        std::vector<Entity::Action> insertActionGlobal;

        for (auto& entity : entities) {
            entity->runActions(insertActionGlobal);
            entity->update();
        }

        std::sort(globalActions.begin(), globalActions.end());
        for (int i = globalActions.size() - 1; i >= 0; i--) {
            if (globalActions[i].time.elapsed() >= 0) { // started
                runAction(globalActions[i], insertActionGlobal);
                globalActions[i].deleteFlag = true;
            }
        }
        for (auto& action : insertActionGlobal) {
            globalActions.push_back(action); // rerun next round
        }
        std::sort(globalActions.begin(), globalActions.end());
        for (int i = globalActions.size() - 1; i >= 0; i--) {
            if (globalActions[i].deleteFlag)
                globalActions.resize(i);
        }
    }

    virtual void runAction(Entity::Action& action, std::vector<Entity::Action>& insertActionGlobal) {
        std::stringstream ss(action.action); 
        std::string function;
        while (ss >> function) {
            if (function == "lay_egg") {
                coord position;
                ss >> position.x >> position.y;
                Egg* egg = new Egg();
                egg->position = position;
                egg->actions.push_back(Entity::Action(Timer::getNow(), "hop"));
                egg->actions.push_back(Entity::Action(Timer::getNow() + getRand() * 5., "hatch"));
                entities.push_back(egg);
            }
            if (function == "hatch") {
                coord position;
                ss >> position.x >> position.y;
                Duck* duck = new Duck();
                duck->position = position;
                duck->actions.push_back(Entity::Action(Timer::getNow(), "hop"));
                // duck->actions.push_back(Entity::Action(Timer::getNow() + getRand() * 5., "lay_egg"));
                duck->actions.push_back(Entity::Action(Timer::getNow() + 1., "duckwalk_to_until " + toString(position.x + 3. * (getRand() - 0.5)) + " " + toString(position.y + 3. * (getRand() - 0.5))));

                entities.push_back(duck);
                ducks.push_back(duck);
            }
        }
    }

    void render() {
        for (auto entity : entities) {
            entity->pushQuads();
        }
    }

};

#endif