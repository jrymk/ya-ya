#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <fstream>
#include <map>
#include "entity.hpp"
#include "duck.hpp"
#include "objects.hpp"
#include "serialization.hpp"

class Game {
    std::vector<Action> actionList;

public:
    std::map<std::string, Duck*> ducks;
    std::map<std::string, Entity*> entities;
    
    void update() {
        for (auto& entity : entities)
            entity.second->update();

        runActions();
    }

    void runActions() {
        std::vector<Action> followUpActions;

        std::sort(actionList.begin(), actionList.end());
        for (int i = actionList.size() - 1; i >= 0; i--) {
            if (actionList[i].time.elapsed() >= 0) { // started
                runAction(actionList[i], followUpActions);
                actionList[i].deleteFlag = true;
            }
        }
        for (auto& action : followUpActions)
            actionList.push_back(action); // will run next update

        std::sort(actionList.begin(), actionList.end());
        for (int i = actionList.size() - 1; i >= 0; i--) {
            if (actionList[i].deleteFlag)
                actionList.resize(i);
        }
    }

    virtual void runAction(Action& action, std::vector<Action>& followUpActions) {
        if (action.id != "game") {
            std::map<std::string, Entity*>::iterator result = entities.find(action.id);
            if (result == entities.end()) {
                debug << "Attempt to find entity " << action.id << " failed";
                // umm... now what?
                return;
            }
            result->second->runAction(action, followUpActions);
            return;
        }
        // global actions
        std::stringstream ss(action.action); 
        std::string function;
        while (ss >> function) {
            if (function == "lay_egg") {
                coord position;
                ss >> position.x >> position.y;
                Egg* egg = new Egg();
                egg->id = "[egg]" + toStr(rand());
                egg->position = position;
                pushAction(egg->id, Timer::getNow(), "hop");
                pushAction(egg->id, Timer::getNow() + getRand() * 10., "hatch");
                entities.insert({egg->id, egg});
            }
            if (function == "hatch") {
                coord position;
                ss >> position.x >> position.y;
                Duck* duck = new Duck();
                duck->id = "[duck]" + toStr(rand());
                duck->position = position;
                pushAction(duck->id, Timer::getNow(), "hop");
                // duck->actions.push_back(Action(Timer::getNow() + getRand() * 5., "lay_egg"));
                pushAction(duck->id, Timer::getNow() + .5, "duckwalk_to_until " + toStr(position.x + 3. * (getRand() - 0.5)) + " " + toStr(position.y + 3. * (getRand() - 0.5)));

                entities.insert({duck->id, duck});
                ducks.insert({duck->id, duck});
            }
        }
    }

    void pushAction(std::string id, Timer timer, std::string action) {
        actionList.push_back(Action(id, timer, action));
    }

    void render() {
        for (auto entity : entities) {
            entity.second->pushQuads();
        }

        std::stringstream ss;
        for (auto a : actionList) {
            ss << std::right << std::setw(10) << std::setprecision(3) << std::fixed << a.time.elapsed() << "s  " << std::left << std::setw(20) << a.id << "  " << a.action << "\n";
        }
        Graphics::drawText(ss.str(), sf::Color::Cyan, 12, UIVec(2., 55.), 0., sf::Color(0, 0, 0, 100), 2.);
    }

    static constexpr const char* defaultFilePath = ".\\save.ya";
    void save() {
        std::ofstream fout(defaultFilePath);
        if(!fout.is_open()) std::cerr << "file saving failed";

        fout << Serialization::serialize<std::map<std::string, Duck*>>(ducks);
        if(fout.bad()) std::cerr << "file saving failed";
        fout.close();
    }

    void load(const char* filepath = defaultFilePath) {
        std::ifstream fin(filepath);
        if(!fin.is_open()) std::cerr << "file loading failed";

        std::string str;
        fin >> str;
        if(fin.bad()) std::cerr << "file loading failed";
        Serialization::deserialize<std::map<std::string, Duck*>>(ducks, str);
        for(auto& p : ducks) entities.insert(p);

        fin.close();
    }
};

#endif