#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <fstream>
#include <map>
#include "entity.hpp"
#include "duck.hpp"
#include "objects.hpp"
#include "serialization.hpp"

class Game {

public:
    std::vector<Action> actionList;
    double updateTime;
    std::map<std::string, Duck*> ducks;
    std::map<std::string, Entity*> entities;
    
    void update() {
        Timer updateTimer;
        for (auto& entity : entities)
            entity.second->update();

        runActions();
        updateTime = updateTimer.elapsed();
    }

    void runActions() {
        std::vector<Action> followUpActions;

        std::sort(actionList.begin(), actionList.end());
        for (int i = actionList.size() - 1; i >= 0; i--) {
            if (actionList[i].time.elapsed() >= 0 && !actionList[i].ranFlag) { // started
                runAction(actionList[i], followUpActions);
                actionList[i].ranFlag = true;
            }
            if (actionList[i].time.elapsed() >= 0.) {
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
                egg->id = "egg$" + makeId();
                egg->position = position;
                pushAction(egg->id, Timer::getNow(), "hop");
                pushAction(egg->id, Timer::getNow() + getRand() * 10., "hatch");
                entities.insert({egg->id, egg});
            }
            if (function == "hatch") {
                coord position;
                ss >> position.x >> position.y;
                Duck* duck = new Duck();
                duck->id = "duck$" + makeName();
                duck->position = position;
                pushAction(duck->id, Timer::getNow(), "hop");
                // duck->actions.push_back(Action(Timer::getNow() + getRand() * 5., "lay_egg"));
                pushAction(duck->id, Timer::getNow() + .5, "duckwalk_to_until " + toStr(position.x + 3. * (getRand() - 0.5)) + " " + toStr(position.y + 3. * (getRand() - 0.5)));

                entities.insert({duck->id, duck});
                ducks.insert({duck->id, duck});
            }
            if (function == "process_collision") {
                std::string eid, fid;
                ss >> eid >> fid;
                Entity* e = findEntity(eid);
                Entity* f = findEntity(fid);
                if (!e || !f)
                    return;
                coord delta = f->position - e->position;
                coord move = delta / delta.len() / delta.len() / 3.;
                if (e->historyPosition.size() >= 2 && (e->historyPosition[1].second.len(e->historyPosition[0].second)) /  e->historyPosition[1].first.elapsed(e->historyPosition[0].first) < 0.1)
                    pushAction(e->id, Timer::getNow(), "slide_velocity_distance " + toStr(-move.x) + " " + toStr(-move.y) + " 0.1");
                if (f->historyPosition.size() >= 2 && (f->historyPosition[1].second.len(f->historyPosition[0].second)) /  f->historyPosition[1].first.elapsed(f->historyPosition[0].first) < 0.1)
                    pushAction(f->id, Timer::getNow(), "slide_velocity_distance " + toStr(move.x) + " " + toStr(move.y) + " 0.1");
            }
        }
    }

    Entity* findEntity(std::string id) {
        auto result = entities.find(id);
        if (result == entities.end()) {
            debug << "Find entity failed when tring to find \"" << id << "\"";
            return nullptr;
        }
        return result->second;
    }

    void pushAction(std::string id, Timer timer, std::string action) {
        actionList.push_back(Action(id, timer, action));
    }

    std::string newId(const std::string& type) {
        std::string id;
        if (type == "duck")
            id = type + "$" + makeName();
        else 
            id = type + "$" + makeId();
        if (entities.find(id) == entities.end())
            return id;
        else
            return newId(type);
    }

    void insertEntity(Entity* entity) {
        if (entity->id == "undefined") {
            debug << "Entity insertion failed because name is undefined";
            return;
        }
        if (entities.find(entity->id) == entities.end())
            entities.insert({entity->id, entity});
        else {
            debug << "Entity insertion failed because name \"" << entity->id << "\" is already taken";
            return;
        }
    }

    void render() {
        for (auto entity : entities) {
            entity.second->pushQuads();
        }

        std::stringstream ss;
        for (int i = actionList.size() - 1; i >= 0; i--) {
            ss << std::right << std::setw(10) << std::setprecision(3) << std::fixed << actionList[i].time.elapsed() << "s  " << std::left << std::setw(20) << actionList[i].id << "  " << actionList[i].action << "\n";
            if (i <= int(actionList.size()) - 101) {
                ss << "... (truncated)\n";
                break;
            }
        }

        for (auto entity : entities) {
            Graphics::drawText(entity.first, sf::Color::Black, 14, Camera::getScreenPos(entity.second->position) + UIVec(0., 30.), .5, sf::Color(255, 255, 255, 100), 3.);
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