#include "game.h"
#include <iomanip>
#include <thread>
#include "player.h"
#include "duck.h"
#include "egg.h"

Game::Game(): 
    neighborsFinder(this),
    controls(this)
{}

void Game::update() {
    Timer updateTimer;
    Profiler::timeSplit("gneighborsfinder");
    neighborsFinder.update();

    auto facing = controls.getFacingEntity(player);
    if (facing) {
        Graphics::drawText(facing->getDescriptionStr(), sf::Color::Black, 12., Camera::getScreenPos(facing->position) + UIVec(0., 40.), 0., sf::Color(255, 255, 255, 140), 3.);
        Graphics::insertUserWireframe(
                Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.0 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
                Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.5 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
                Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.0 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
                Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.5 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
                sf::Color(255, 150, 60, 100), sf::Color(0, 0, 0, 100)
        );
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (controls.inventory[0] == nullptr && facing != nullptr) {
            debug << "picked up " << facing->id << "\n";
            controls.inventory[0] = facing;
            {
                Action a(controls.inventory[0], Timer::getNow(), ENTITY_MOTION_FROZEN);
                a.argBool[0] = true;
                pushAction(a);
            }
            {
                Action a(controls.inventory[0], Timer::getNow(), ENTITY_HIGHLIGHTABLE);
                a.argBool[0] = false;
                pushAction(a);
            }
        }
        if (controls.inventory[0] != nullptr) {
            coord pos = player->position
                        + coord(Camera::getAngleVectorUntransformed(0.4, player->heading + PI/4).x, Camera::getAngleVectorUntransformed(0.4, player->heading + PI/4).y);
            {
                Action a(controls.inventory[0], Timer::getNow(), ENTITY_MOVE_TO_APPROACH);
                a.argCoord[0] = pos;
                a.argFloat[0] = .00000005;
                pushAction(a);
            }
            {
                Action a(controls.inventory[0], Timer::getNow(), ENTITY_HEADING_INSTANT);
                a.argFloat[0] = player->heading;
                pushAction(a);
            }
        }
    }
    else {
        if (controls.inventory[0] != nullptr) {
            {
                Action a(controls.inventory[0], Timer::getNow(), ENTITY_MOTION_FROZEN);
                a.argBool[0] = false;
                pushAction(a);
            }
            {
                Action a(controls.inventory[0], Timer::getNow(), ENTITY_HIGHLIGHTABLE);
                a.argBool[0] = true;
                pushAction(a);
            }
            controls.inventory[0] = nullptr;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        if (controls.inventory[1] == nullptr && facing != nullptr) {
            controls.inventory[1] = facing;
            {
                Action a(controls.inventory[1], Timer::getNow(), ENTITY_MOTION_FROZEN);
                a.argBool[0] = true;
                pushAction(a);
            }
            {
                Action a(controls.inventory[1], Timer::getNow(), ENTITY_HIGHLIGHTABLE);
                a.argBool[0] = false;
                pushAction(a);
            }
        }
        if (controls.inventory[1] != nullptr) {
            coord pos = player->position
                        + coord(Camera::getAngleVectorUntransformed(0.4, player->heading - PI/4).x, Camera::getAngleVectorUntransformed(0.4, player->heading - PI/4).y);
            {
                Action a(controls.inventory[1], Timer::getNow(), ENTITY_MOVE_TO_APPROACH);
                a.argCoord[0] = pos;
                a.argFloat[0] = .00000005;
                pushAction(a);
            }
            {
                Action a(controls.inventory[1], Timer::getNow(), ENTITY_HEADING_INSTANT);
                a.argFloat[0] = player->heading;
                pushAction(a);
            }
        }
    }
    else {
        if (controls.inventory[1] != nullptr) {
            {
                Action a(controls.inventory[1], Timer::getNow(), ENTITY_MOTION_FROZEN);
                a.argBool[0] = false;
                pushAction(a);
            }
            {
                Action a(controls.inventory[1], Timer::getNow(), ENTITY_HIGHLIGHTABLE);
                a.argBool[0] = true;
                pushAction(a);
            }
            controls.inventory[1] = nullptr;
        }
    }


    Profiler::timeSplit("gprocesscollisions");
    processCollisions();

    Profiler::timeSplit("gentitiesupdate");
    for (auto& entity : entities) {
        entity.second->update();
    }
    Profiler::timeSplit("grunactions");
    runActions();
    updateTime = updateTimer.elapsed();
}

void Game::processCollisions() {
    for (auto e : entities) {
        // auto eid = splitId(e.first);
        if (!e.second->collisionCollidable)
            continue;
        
        auto result = neighborsFinder.findNeighbors(e.second->position, .3, DUCK);
        
        for (auto f : result) {
            if (e.second == f)
                continue;
            if (!f->collisionPushable)
                continue;

            if (e.second->position.len(f->position) < .3) {
                Action a(Timer::getNow(), GLOBAL_PROCESS_COLLISION);
                a.argEntity[0] = e.second;
                a.argEntity[1] = f;
                pushAction(a);
            }
        }
    }
}

void Game::runActions() {
    std::vector<Action> followUpActions;

    Profiler::timeSplit("sort");
    // std::sort(actionList.begin(), actionList.end());
    // std::vector<std::thread> threadPool;
    Profiler::timeSplit("run");
    // for (int i = actionList.size() - 1; i >= 0; i--) {
    while (!actionList.empty()) {
        Action action = actionList.top();
        if (action.time.elapsed() >= 0 && !actionList.top().ranFlag) { // started
//            std::cerr << std::right << std::setw(10) << std::setprecision(3) << std::fixed << Timer::getGlobalStart().elapsed(action.time) << "s  " << std::left << std::setw(20) << (action.isGlobal ? "global" : action.entity->id) << "  " << action.command << "\n";
            // threadPool.push_back(std::thread(&Game::runAction, this, actionList[i], followUpActions));
            runAction(action, followUpActions);
            // actionList[i].ranFlag = true;
            // actionList[i].deleteFlag = true;
            actionList.pop();
        }
        else 
            break;
    }
    // for (std::thread& t : threadPool) {
    //     t.join();
    // }
    Profiler::timeSplit("followup");
    for (auto& action : followUpActions)
        actionList.push(action); // will run next update

    Profiler::timeSplit("ssort");
    // std::sort(actionList.begin(), actionList.end());
    Profiler::timeSplit("resize");
    // for (int i = actionList.size() - 1; i >= 0; i--) {
    //     if (actionList[i].deleteFlag)
    //         actionList.resize(i);
    // }
}

void Game::runAction(Action& action, std::vector<Action>& followUpActions) {
    // debug << std::setw(100) << action.action << std::setw(20) << action.entity << "\t" << (action.entity == nullptr ? "global" : action.entity->id) << std::endl;
    if (!action.isGlobal) {
        action.entity->runActionEntity(action, followUpActions); // first go through entity
        return;
    }
    // global actions
    switch (action.command) {
        case GLOBAL_LAY_UNFERTILIZED_EGG: {
            std::shared_ptr<Egg> egg(new Egg);
            egg->id = newId(EGG);
            egg->childClassPtr = egg;
            auto& egg_ptr = insertEntity(egg);
            egg->position = action.argCoord[0] + coord(0.01 * (getRand() - .5), 0.01 * (getRand() - .5));
            pushAction(Action(egg_ptr, Timer::getNow(), ENTITY_HOP));
            break;
        }
        case GLOBAL_LAY_FERTILIZED_EGG: {
            std::shared_ptr<Egg> egg(new Egg);
            egg->id = newId(EGG);
            egg->childClassPtr = egg;
            auto& egg_ptr = insertEntity(egg);
            egg->position = action.argCoord[0] + coord(0.01 * (getRand() - .5), 0.01 * (getRand() - .5));
            egg->fertilized = true;
            egg->genderIsMale = action.argBool[0];
            pushAction(Action(egg_ptr, Timer::getNow(), ENTITY_HOP));
            pushAction(Action(egg_ptr, Timer::getNow() + getRand() * 10., EGG_HATCH));
            break;
        }
        case GLOBAL_HATCH: {
            if (getRand() > .8) // 60% success
                return;
            std::shared_ptr<Duck> duck(new Duck);
            duck->id = newId(DUCK);
            duck->childClassPtr = duck;
            auto& duck_ptr = insertEntity(duck);
            duck->position = action.argCoord[0] + coord(0.05 * (getRand() - .5), 0.05 * (getRand() - .5));
            duck->genderIsMale = action.argBool[0];
            pushAction(Action(duck_ptr, Timer::getNow(), ENTITY_HOP));

            Action a(duck_ptr, Timer::getNow() + .5, DUCK_DUCKWALK_TO_UNTIL);
            a.argCoord[0].x = action.argCoord[0].x + 3. * (getRand() - 0.5);
            a.argCoord[0].y = action.argCoord[0].y + 3. * (getRand() - 0.5);
            pushAction(a);
            break;
        }
        case GLOBAL_DESTROY: {
            destroyEntity(action.argString[0]);
            break;
        }
        case GLOBAL_PROCESS_COLLISION: {
            if (!action.argEntity[0]->collisionCollidable || !action.argEntity[1]->collisionPushable)
                return;
            coord delta = action.argEntity[1]->position - action.argEntity[0]->position;
            coord move = delta / delta.len() / delta.len() / 3.;
//            if (action.argEntity[0]->historyPosition.size() >= 2 &&
//                (action.argEntity[0]->historyPosition[1].second.len(action.argEntity[0]->historyPosition[0].second)) /  action.argEntity[0]->historyPosition[1].first.elapsed(action.argEntity[0]->historyPosition[0].first) < 0.1) {
//                Action a(action.argEntity[0], Timer::getNow(), ENTITY_SLIDE_VELOCITY_DISTANCE);
//                a.argCoord[0] = move * -1.;
//                a.argFloat[0] = 0.1;
//                pushAction(a);
//            }
            if (action.argEntity[1]->historyPosition.size() >= 2 &&
                (action.argEntity[1]->historyPosition[1].second.len(action.argEntity[1]->historyPosition[0].second)) /  action.argEntity[1]->historyPosition[1].first.elapsed(action.argEntity[1]->historyPosition[0].first) < 0.1) {
                Action a(action.argEntity[1], Timer::getNow(), ENTITY_SLIDE_VELOCITY_DISTANCE);
                a.argCoord[0] = move;
                a.argFloat[0] = 0.1;
                pushAction(a);
            }
            break;
        }
        case GLOBAL_FIND_MATE_FEMALE: {
            auto result = neighborsFinder.findNeighbors(action.argEntity[0]->position, 8.);
            std::shared_ptr<Entity> closest;
            std::vector<std::shared_ptr<Entity>> candidates;
            for (auto f : result) {
                if (!f->childClassPtr)
                    continue;
                if (f->type != DUCK)
                    continue;
                Duck* duck = dynamic_cast<Duck*>(f->childClassPtr.get());
                if (!duck)
                    continue;
                if (!duck->genderIsMale) {
                    if (!closest || f->position.len(action.argEntity[0]->position) < closest->position.len(action.argEntity[0]->position))
                        closest = f;
                    candidates.push_back(f);
                }
            }
            if (closest) {
                if (getRand() < .4) // change mate?
                    closest = candidates[std::min(int(getRand() * candidates.size()), int(candidates.size() - 1))];
                Action a(action.argEntity[0], Timer::getNow(), DUCK_RESULT_FIND_MATE_FEMALE);
                a.argEntity[0] = closest;
                a.argCoord[0] = closest->position;
                pushAction(a);
            }
            break;
        }
        case GLOBAL_LAY_EGG_FIND_NEARBY_MALE: {
            auto result = neighborsFinder.findNeighbors(action.argEntity[0]->position, 1.);
            std::vector<std::shared_ptr<Entity>> candidates;
            for (auto f : result) {
                if (!f->childClassPtr)
                    continue;
                if (f->type != DUCK)
                    continue;
                auto duck = std::dynamic_pointer_cast<Duck>(f->childClassPtr).get();
                if (!duck)
                    continue;
                if (!duck->genderIsMale) {
                    // debug << duck->id << " is " << (duck->genderIsMale ? "male" : "female") << "\n";
                    candidates.push_back(f);
                }
            }
            if (candidates.size() == 0) {
                followUpActions.push_back(Action(action.argEntity[0], Timer::getNow(), DUCK_LAY_UNFERTILIZED_EGG));
                return;
            }
            std::shared_ptr<Entity> f = candidates[0];
            f = candidates[std::min(int(getRand() * candidates.size()), int(candidates.size() - 1))];
            Action a(action.argEntity[0], Timer::getNow(), DUCK_HAVE_SEX_WITH);
            a.argEntity[0] = f;
            pushAction(a);
            Action b(f, Timer::getNow(), DUCK_HAVE_SEX_WITH);
            a.argEntity[0] = action.argEntity[0];
            pushAction(a);
            break;
        }
    }
}

std::shared_ptr<Entity> Game::findEntity(std::string id) {
    auto result = entities.find(id);
    if (result == entities.end()) {
        debug << "Find entity failed when tring to find \"" << id << "\"\n";
        return nullptr;
    }
    return result->second;
}

void Game::pushAction(const Action& action) {
    actionList.push(action);
}

std::string Game::newId(EntityType type) {
    std::string id;
    switch (type) {
    case ENTITY:
        id = "undefined$" + randomId();
        break;
    case PLAYER:
        id = "player$player";
        break;
    case DUCK:
        id = "duck$" + randomName();
        break;
    case EGG:
        id = "EGG$" + randomId();
        break;
    }
    if (entities.find(id) == entities.end())
        return id;
    else
        return newId(type);
}



void Game::destroyEntity(std::string id) {
    auto result = entities.find(id);
    if (result == entities.end()) {
        debug << "Find entity failed when tring to find \"" << id << "\"\n";
        return;
    }
    result->second->deleted = true;
    neighborsFinder.destroyEntry(result->second);
    delete result->second->childClassPtr.get();
    entities.erase(result);
}

void Game::render() {
    Profiler::timeSplit("pushquads");
    for (auto entity : entities) {
        entity.second->childClassPtr->pushQuads();
        //        switch (entity.second->type) {
//            case PLAYER:
//                std::dynamic_pointer_cast<Player>(entity.second->childClassPtr)->pushQuads();
//                break;
//            case DUCK:
//                std::dynamic_pointer_cast<Duck>(entity.second->childClassPtr)->pushQuads();
//                break;
//            case EGG:
//                std::dynamic_pointer_cast<Egg>(entity.second->childClassPtr)->pushQuads();
//                break;
//        }
    }
    Profiler::timeSplit("showactionlist");
    // if (showActionList) {
    //     std::stringstream ss;
    //     for (int i = actionList.size() - 1; i >= 0; i--) {
    //         ss << std::right << std::setw(10) << std::setprecision(3) << std::fixed << actionList[i].time.elapsed() << "s  " << std::left << std::setw(20) << (actionList[i].entity == nullptr ? "global" : actionList[i].entity->id) << "  " << actionList[i].command << "\n";
    //         if (i <= int(actionList.size()) - 101) {
    //             ss << "... (truncated)\n";
    //             break;
    //         }
    //     }
    //     Graphics::drawText(ss.str(), sf::Color::Cyan, 12, UIVec(2., 55.), 0., sf::Color(0, 0, 0, 100), 2.);
    // }

    // for (auto entity : entities) {
    //     Graphics::drawText(entity.first, sf::Color::Black, 14, Camera::getScreenPos(entity.second->position) + UIVec(0., 30.), .5, sf::Color(255, 255, 255, 100), 3.);
    // }
    Profiler::timeSplit("getfacingentity");
    auto facing = controls.getFacingEntity(player);
    if (facing) {
        Graphics::drawText(facing->getDescriptionStr(), sf::Color::Black, 12., Camera::getScreenPos(facing->position) + UIVec(0., 40.), 0., sf::Color(255, 255, 255, 140), 3.);
        Graphics::insertUserWireframe(
            Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.0 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
            Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.5 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
            Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.0 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
            Camera::getScreenPos(facing->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.5 * PI) + UIVec(0, -facing->zPosition * Camera::getScale()),
            sf::Color(255, 150, 60, 100), sf::Color(0, 0, 0, 100)
        );
    }
}

static constexpr const char* defaultFilePath = ".\\save.ya";
void Game::save() {
    std::ofstream fout(defaultFilePath);
    if(!fout.is_open()) std::cerr << "file saving failed";

    fout << Serialization::serialize<std::map<std::string, std::shared_ptr<Entity> > >(entities);
    if(fout.bad()) std::cerr << "file saving failed";
    fout.close();
}

void Game::load(const char* filepath) {
    std::ifstream fin(filepath);
    if(!fin.is_open()) std::cerr << "file loading failed";

    std::string str;
    fin >> str;
    if(fin.bad()) std::cerr << "file loading failed";

    fin.close();
}

void Game::setPlayer(std::shared_ptr<Entity> &player) {
    this->player = player;
}
