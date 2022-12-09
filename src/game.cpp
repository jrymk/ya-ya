#include "game.h"
#include <iomanip>
#include <thread>

Game::Game(): 
    neighborsFinder(this),
    controls(&entities, &neighborsFinder)
{}

void Game::update() {
    Timer updateTimer;
    neighborsFinder.update();

    auto player = findEntity("player$player");
    if (!player)
        return;
    auto facing = controls.getFacingEntity(player);
    std::string facingId = "null";
    if (facing) {
        Graphics::drawText(facing->getDescriptionStr(), sf::Color::Black, 12., Camera::getScreenPos(facing->position) + UIVec(0., 40.), 0., sf::Color(255, 255, 255, 140), 3.);
        Graphics::insertUserWireframe(
            Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.0 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
            Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.5 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
            Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.0 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
            Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.5 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
            sf::Color(255, 150, 60, 100), sf::Color(0, 0, 0, 100)
        );
        facingId = facing->id;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (controls.inventory[0] == "null" && facingId != "null") {
            controls.inventory[0] = facingId;
            pushAction(controls.inventory[0], Timer::getNow(), "motion_frozen 1");
            pushAction(controls.inventory[0], Timer::getNow(), "highlightable 0");
        }
        if (controls.inventory[0] != "null") {
            coord pos = player->position 
                + coord(Camera::getAngleVectorUntransformed(0.4, player->heading + PI/4).x, Camera::getAngleVectorUntransformed(0.4, player->heading + PI/4).y);
            pushAction(controls.inventory[0], Timer::getNow(), "move_to_approach " + toStr(pos.x) + " " + toStr(pos.y) + " 0.00000005");
            pushAction(controls.inventory[0], Timer::getNow(), "heading_instant " + toStr(player->heading));
        }
    }
    else {
        if (controls.inventory[0] != "null") {
            pushAction(controls.inventory[0], Timer::getNow(), "motion_frozen 0");
            pushAction(controls.inventory[0], Timer::getNow(), "highlightable 1");
            controls.inventory[0] = "null";
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        if (controls.inventory[1] == "null" && facingId != "null") {
            controls.inventory[1] = facingId;
            pushAction(controls.inventory[1], Timer::getNow(), "motion_frozen 1");
            pushAction(controls.inventory[1], Timer::getNow(), "highlightable 0");
        }
        if (controls.inventory[1] != "null") {
            coord pos = player->position 
                + coord(Camera::getAngleVectorUntransformed(0.4, player->heading - PI/4).x, Camera::getAngleVectorUntransformed(0.4, player->heading - PI/4).y);
            pushAction(controls.inventory[1], Timer::getNow(), "move_to_approach " + toStr(pos.x) + " " + toStr(pos.y) + " 0.00000005");
            pushAction(controls.inventory[1], Timer::getNow(), "heading_instant " + toStr(player->heading));
        }
    }
    else {
        if (controls.inventory[1] != "null") {
            pushAction(controls.inventory[1], Timer::getNow(), "motion_frozen 0");
            pushAction(controls.inventory[1], Timer::getNow(), "highlightable 1");
            controls.inventory[1] = "null";
        }
    }


    processCollisions();

    for (auto& entity : entities)
        entity.second->update();
    runActions();

    updateTime = updateTimer.elapsed();
}

void Game::processCollisions() {
    for (auto e : entities) {
        auto eid = splitId(e.first);
        if (eid.first != "duck" && eid.first != "player")
            continue;
        
        auto result = neighborsFinder.findNeighbors(e.second->position, .3, "duck");
        for (auto f : result) {
            if (e.second == f)
                continue;
            if (f->type != "duck")
                continue;

            if (e.second->position.len(f->position) < .3) {
                pushAction("global", Timer::getNow(), "process_collision " + e.second->id + " " + f->id);
            }
        }
    }
}

void Game::runActions() {
    std::vector<Action> followUpActions;

    // std::sort(actionList.begin(), actionList.end());
    // std::vector<std::thread> threadPool;
    // for (int i = actionList.size() - 1; i >= 0; i--) {
    while (!actionList.empty()) {
        Action action = actionList.top();
        if (action.time.elapsed() >= 0 && !action.ranFlag) { // started
            std::cerr << std::right << std::setw(10) << std::setprecision(3) << std::fixed << Timer::getGlobalStart().elapsed(action.time) << "s  " << std::left << std::setw(20) << action.id << "  " << action.action << "\n";
            // threadPool.push_back(std::thread(&Game::runAction, this, actionList[i], followUpActions));
            runAction(action, followUpActions);
            action.ranFlag = true;
            action.deleteFlag = true;
            actionList.pop();
        }
        else 
            break;
    }
    // for (std::thread& t : threadPool) {
    //     t.join();
    // }
    for (auto& action : followUpActions) {
        actionList.push(action); // will run next update
        if (actionList.size() > 1000) {
            debug << "Actions overloaded, check log. Some actions may be lost\n";
            break;
        }
    }

    // std::sort(actionList.begin(), actionList.end());
    // for (int i = actionList.size() - 1; i >= 0; i--) {
    //     if (actionList[i].deleteFlag)
    //         actionList.resize(i);
    // }
}

void Game::runAction(Action& action, std::vector<Action>& followUpActions) {
    if (action.id != "global") {
        std::map<std::string, Entity*>::iterator result = entities.find(action.id);
        if (result == entities.end()) {
            debug << "Attempt to find entity " << action.id << " failed\n";
            // umm... now what?
            return;
        }
        result->second->runActionCommon(action, followUpActions);
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
        else if (function == "lay_fertilized_egg") {
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
        else if (function == "hatch") {
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
        else if (function == "destroy") {
            std::string id;
            ss >> id;
            destroyEntity(id);
        }
        else if (function == "process_collision") {
            std::string eid, fid;
            ss >> eid >> fid;
            Entity* e = findEntity(eid);
            Entity* f = findEntity(fid);
            if (!e || !f)
                return;
            coord delta = f->position - e->position;
            coord move = delta / delta.len() / delta.len() / 3.;
            if (e->historyPosition.size() >= 2 && (e->historyPosition[1].second.len(e->historyPosition[0].second)) /  e->historyPosition[1].first.elapsed(e->historyPosition[0].first) < 0.1 && !e->motionFrozen)
                pushAction(e->id, Timer::getNow(), "slide_velocity_distance " + toStr(-move.x) + " " + toStr(-move.y) + " 0.1");
            if (f->historyPosition.size() >= 2 && (f->historyPosition[1].second.len(f->historyPosition[0].second)) /  f->historyPosition[1].first.elapsed(f->historyPosition[0].first) < 0.1 && !f->motionFrozen)
                pushAction(f->id, Timer::getNow(), "slide_velocity_distance " + toStr(move.x) + " " + toStr(move.y) + " 0.1");
        }
        else if (function == "find_mate_female") {
            std::string id;
            ss >> id;
            Entity* e = findEntity(id);
            if (!e)
                return;
            auto result = neighborsFinder.findNeighbors(e->position, 8.);
            Entity* closest = nullptr;
            std::vector<Entity*> candidates;
            for (auto f : result) {
                if (!f->childClassPtr)
                    continue;
                if (f->type != "duck")
                    continue;
                Duck* duck = dynamic_cast<Duck*>(f->childClassPtr);
                if (!duck)
                    continue;
                if (!duck->genderIsMale) {
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
        else if (function == "lay_egg_find_nearby_male") {
            std::string eid;
            ss >> eid;
            Entity* e = findEntity(eid);
            if (!e)
                return;
            auto result = neighborsFinder.findNeighbors(e->position, 1.);
            std::vector<Entity*> candidates;
            for (auto f : result) {
                if (!f->childClassPtr)
                    continue;
                if (f->type != "duck")
                    continue;
                Duck* duck = dynamic_cast<Duck*>(f->childClassPtr);
                if (!duck)
                    continue;
                if (!duck->genderIsMale) {
                    candidates.push_back(f);
                }
            }
            if (candidates.size() == 0) {
                followUpActions.push_back(Action(e->id, Timer::getNow(), "lay_unfertilized_egg"));
                return;
            }
            Entity* f = candidates[0];
            f = candidates[std::min(int(getRand() * candidates.size()), int(candidates.size() - 1))];
            pushAction(e->id, Timer::getNow(), "have_sex_with " + f->id);
            pushAction(f->id, Timer::getNow(), "have_sex_with " + e->id);
        }
    }
}

Entity* Game::findEntity(const std::string& id) {
    auto result = entities.find(id);
    if (result == entities.end()) {
        debug << "Find entity failed when tring to find \"" << id << "\"\n";
        return nullptr;
    }
    return result->second;
}

void Game::pushAction(std::string id, Timer timer, std::string action) {
    actionList.push(Action(id, timer, action));
}

std::string Game::newId(const std::string& type) {
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

void Game::insertEntity(Entity* entity) {
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

void Game::destroyEntity(std::string id) {
    auto result = entities.find(id);
    if (result == entities.end()) {
        debug << "Find entity failed when tring to find \"" << id << "\"\n";
        return;
    }
    neighborsFinder.update();
    neighborsFinder.destroyEntry(result->second);
    delete result->second->childClassPtr;
    entities.erase(result);
}

void Game::render() {
    for (auto entity : entities) {
        entity.second->pushQuads();
    }
}

static constexpr const char* defaultFilePath = ".\\save.ya";
void Game::save() {
    std::ofstream fout(defaultFilePath);
    if(!fout.is_open()) std::cerr << "file saving failed";

    fout << Serialization::serialize<std::map<std::string, Entity*> >(entities);
    if(fout.bad()) std::cerr << "file saving failed";
    fout.close();
}

void Game::load(const char* filepath) {
    std::ifstream fin(filepath);
    if(!fin.is_open()) std::cerr << "file loading failed";

    std::string str;
    fin >> str;
    if(fin.bad()) std::cerr << "file loading failed";
    //Serialization::deserialize<std::map<std::string, Entity*>>(entities, str);
    //for(auto& p : ducks) entities.insert(p);

    fin.close();
}