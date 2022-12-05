#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <fstream>
#include <map>
#include <set>
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
    
    class NeighborsFinder {
        Game* game;
        std::map<std::pair<int, int>, std::set<std::string>> chunkMembers[5];
    
    public:
        std::pair<int, int> getChunk(coord c, int level) {
            return std::make_pair(int(floor(c.x)) & (~0 << level), int(floor(c.y)) & (~0 << level));
        }
        std::pair<int, int> getChunk(std::pair<int, int> p, int level) {
            return std::make_pair(p.first & (~0 << level), p.second & (~0 << level));
        }

        void update() {
            for (auto e : game->entities) {
                if (e.second->neighborsFinderMyTile == std::make_pair((int)-1e8, (int)1e8)) {
                    e.second->neighborsFinderMyTile = getChunk(e.second->position, 0);
                    for (int l = 0; l < 5; l++)
                        chunkMembers[l][getChunk(e.second->position, l)].insert(e.first);
                }
                else {
                    for (int l = 0; l < 5; l++) {
                        if (e.second->neighborsFinderMyTile != getChunk(e.second->position, l)) {
                            chunkMembers[l][getChunk(e.second->neighborsFinderMyTile, l)].erase(e.second->id);
                            chunkMembers[l][getChunk(e.second->position, l)].insert(e.second->id);
                            if (l == 0)
                                e.second->neighborsFinderMyTile = getChunk(e.second->position, 0);
                        } 
                        else
                            break;
                    }
                }
            }
        }

        void destroyEntry(Entity* e) {
            for (int l = 0; l < 5; l++)
                chunkMembers[l][getChunk(e->neighborsFinderMyTile, l)].erase(e->id);
        }

        std::vector<Entity*> findNeighbors(coord center, double radius, std::string filter = "") {
            std::vector<Entity*> neighbors;
            int d = int(ceil(radius));
            int l = 0;
            for (; l < 4; l++) {
                if (d <= (1 << l))
                    break;
            }
            for (int x = ((int(floor(center.x - radius)) - (1 << l)) & (~0 << l)); x <= ((int(ceil(center.x + radius)) + (1 << l)) & (~0 << l)); x+=(1 << l)) {
                for (int y = ((int(floor(center.y - radius)) - (1 << l)) & (~0 << l)); y <= ((int(ceil(center.y + radius)) + (1 << l)) & (~0 << l)); y+=(1 << l)) {
                    if (chunkMembers[l].find(std::make_pair(x, y)) == chunkMembers[l].end())
                        continue;
                    for (auto& id : chunkMembers[l][std::make_pair(x, y)]) {
                        Entity* e = game->findEntity(id);
                        if (e) {
                            // if (filter != "" && e->type != filter)
                            //     continue;
                            if (e->position.len(center) <= radius)
                                neighbors.push_back(e);
                        }
                    }
                }
            }
            return neighbors;
        }

        NeighborsFinder(Game* game): game(game) {}

    } neighborsFinder;

    Game(): neighborsFinder(this) {
        
    }

    void update() {
        Timer updateTimer;
        neighborsFinder.update();

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
            if (actionList[i].time.elapsed() >= 1.) {
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
        if (action.id != "global") {
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
        if (ss >> function) {
            if (function == "lay_unfertilized_egg") {
                coord position;
                ss >> position.x >> position.y;
                Egg* egg = new Egg();
                egg->id = newId("egg");
                egg->position = position + coord(0.01 * (getRand() - .5), 0.01 * (getRand() - .5));
                pushAction(egg->id, Timer::getNow(), "hop");
                // pushAction(egg->id, Timer::getNow() + getRand() * 10., "hatch"); // they don't hatch
                insertEntity(egg);
            }
            if (function == "lay_fertilized_egg") {
                coord position;
                bool genderIsMale;
                ss >> position.x >> position.y >> genderIsMale;
                Egg* egg = new Egg();
                egg->id = newId("egg");
                egg->position = position + coord(0.01 * (getRand() - .5), 0.01 * (getRand() - .5));
                egg->fertilized = true;
                egg->genderIsMale = genderIsMale;
                pushAction(egg->id, Timer::getNow(), "hop");
                pushAction(egg->id, Timer::getNow() + getRand() * 10., "hatch");
                insertEntity(egg);
            }
            if (function == "hatch") {
                if (getRand() > .8) // 60% success
                    return;
                coord position;
                bool genderIsMale;
                ss >> position.x >> position.y >> genderIsMale;
                Duck* duck = new Duck();
                duck->id = newId("duck");
                duck->position = position + coord(0.05 * (getRand() - .5), 0.05 * (getRand() - .5));
                duck->genderIsMale = genderIsMale;
                pushAction(duck->id, Timer::getNow(), "hop");
                // duck->actions.push_back(Action(Timer::getNow() + getRand() * 5., "lay_egg"));
                pushAction(duck->id, Timer::getNow() + .5, "duckwalk_to_until " + toStr(position.x + 3. * (getRand() - 0.5)) + " " + toStr(position.y + 3. * (getRand() - 0.5)));
                insertEntity(duck);
            }
            if (function == "destroy") {
                std::string id;
                ss >> id;
                destroyEntity(id);
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
            if (function == "find_mate_female") {
                std::string id;
                ss >> id;
                Entity* e = findEntity(id);
                if (!e)
                    return;
                auto result = neighborsFinder.findNeighbors(e->position, 8.);
                Entity* closest = nullptr;
                std::vector<Entity*> candidates;
                for (auto f : result) {
                    if (!f->genderIsMale) {
                        if (!closest || f->position.len(e->position) < closest->position.len(e->position))
                            closest = f;
                        candidates.push_back(f);
                    }
                }
                if (closest) {
                    if (getRand() < .4) // change mate?
                        closest = candidates[std::min(int(getRand() * candidates.size()), int(candidates.size() - 1))];
                    pushAction(e->id, Timer::getNow(), "result_find_mate_female " + closest->id + " " + toStr(closest->position.x) + " " + toStr(closest->position.y));
                }
            }
            if (function == "lay_egg_find_nearby_male") {
                std::string eid;
                ss >> eid;
                Entity* e = findEntity(eid);
                if (!e)
                    return;
                auto result = neighborsFinder.findNeighbors(e->position, 1.);
                std::vector<Entity*> candidates;
                for (auto f : result) {
                    if (f->genderIsMale) 
                        candidates.push_back(f);
                }
                if (candidates.size() == 0) {
                    followUpActions.push_back(Action(e->id, Timer::getNow(), "lay_unfertilized_egg"));
                    return;
                }
                Entity* f = candidates[
                f = candidates[std::min(int(getRand() * candidates.size()), int(candidates.size() - 1))];
                pushAction(e->id, Timer::getNow(), "have_sex_with " + f->id);
                pushAction(f->id, Timer::getNow(), "have_sex_with " + e->id);
            }
        }
    }

    Entity* findEntity(std::string id) {
        auto result = entities.find(id);
        if (result == entities.end()) {
            debug << "Find entity failed when tring to find \"" << id << "\"\n";
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
            id = type + "$" + randomName();
        else 
            id = type + "$" + randomId();
        if (entities.find(id) == entities.end())
            return id;
        else
            return newId(type);
    }

    void insertEntity(Entity* entity) {
        if (entity->id == "undefined") {
            debug << "Entity insertion failed because name is undefined\n";
            return;
        }
        if (entities.find(entity->id) == entities.end()) {
            entities.insert({entity->id, entity});
            pushAction(entity->id, Timer::getNow(), "init");
        }
        else {
            debug << "Entity insertion failed because name \"" << entity->id << "\" is already taken\n";
            return;
        }
    }

    void destroyEntity(std::string id) {
        auto result = entities.find(id);
        if (result == entities.end()) {
            debug << "Find entity failed when tring to find \"" << id << "\"\n";
            return;
        }
        neighborsFinder.destroyEntry(result->second);
        delete result->second;
        entities.erase(result);
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

        // for (auto entity : entities) {
        //     Graphics::drawText(entity.first, sf::Color::Black, 14, Camera::getScreenPos(entity.second->position) + UIVec(0., 30.), .5, sf::Color(255, 255, 255, 100), 3.);
        // }

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