#include "game.h"
#include <iomanip>
#include <thread>
#include "player.h"
#include "duck.h"
#include "egg.h"

Game::Game() :
        neighborsFinder(this),
        controls(this) {}

void Game::update() {
    Timer updateTimer;

    neighborsFinder.update();
    controls.update();

    if (controls.facingEntity) {
        Graphics::drawText(controls.facingEntity->getDescriptionStr(), sf::Color::Black, 12., Camera::getScreenPos(controls.facingEntity->position) + UIVec(0., 40.), 0.,
                           sf::Color(255, 255, 255, 140), 3.);
        Graphics::insertUserWireframe(
                Camera::getScreenPos(controls.facingEntity->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.0 * PI) +
                UIVec(0, -controls.facingEntity->zPosition * Camera::getScale()),
                Camera::getScreenPos(controls.facingEntity->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.5 * PI) +
                UIVec(0, -controls.facingEntity->zPosition * Camera::getScale()),
                Camera::getScreenPos(controls.facingEntity->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.0 * PI) +
                UIVec(0, -controls.facingEntity->zPosition * Camera::getScale()),
                Camera::getScreenPos(controls.facingEntity->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.5 * PI) +
                UIVec(0, -controls.facingEntity->zPosition * Camera::getScale()),
                sf::Color(255, 150, 60, 100), sf::Color(0, 0, 0, 100)
        );
    }

    processCollisions();

    for (auto &entity: entities) {
        entity.second->update();
    }
    runActions();

    for (auto &entity: entities) {
        entity.second->setInventoryProps();
    }

    updateTime = updateTimer.elapsed();
}

void Game::processCollisions() {
    for (auto e: entities) {
        // auto eid = splitId(e.first);
        if (!e.second->collisionCollidable)
            continue;

        auto result = neighborsFinder.findNeighbors(e.second->position, .3, DUCK);

        for (auto f: result) {
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

    while (!actionList.empty()) {
        Action action = actionList.top();
        if (action.time.elapsed() >= 0 && !actionList.top().ranFlag) { // started
//            std::cerr << std::right << std::setw(10) << std::setprecision(3) << std::fixed << Timer::getGlobalStart().elapsed(action.time) << "s  " << std::left << std::setw(20)
//                      << (action.isGlobal ? "global" : action.entity->id) << "  " << std::setw(5) << action.command << "  " << action.caller << "\n";
            runAction(action, followUpActions);
            actionList.pop();
        } else
            break;
    }
    for (auto &action: followUpActions)
        actionList.push(action); // will run next update
}

void Game::runAction(Action &action, std::vector<Action> &followUpActions) {
    if (!action.isGlobal) {
        action.entity->runActionEntity(action, followUpActions); // first go through entity
        return;
    }
    // global actions
    switch (action.command) {
        case GLOBAL_PROCESS_COLLISION: {
            if (!action.argEntity[0]->collisionCollidable || !action.argEntity[1]->collisionPushable)
                return;
            if (action.argEntity[0]->ownedBy == action.argEntity[1] || action.argEntity[1]->ownedBy == action.argEntity[0])
                return;
//            if ((action.argEntity[0]->type == EGG && action.argEntity[1]->type == DUCK) || (action.argEntity[1]->type == EGG && action.argEntity[0]->type == DUCK))
//                return;

            coord delta = action.argEntity[1]->position - action.argEntity[0]->position;
            if (delta.len() < .00001) {
                delta = coord::getRandCoord();
//                debug << "Warning: Processing collision on two stacked entities\n";
            }
            coord move = delta / delta.len() / delta.len() / 3.;
//            if (action.argEntity[0]->historyPosition.size() >= 2 &&
//                (action.argEntity[0]->historyPosition[1].second.len(action.argEntity[0]->historyPosition[0].second)) /  action.argEntity[0]->historyPosition[1].first.elapsed(action.argEntity[0]->historyPosition[0].first) < 0.1) {
//                Action a(action.argEntity[0], Timer::getNow(), ENTITY_SLIDE_VELOCITY_DISTANCE);
//                a.argCoord[0] = move * -1.;
//                a.argFloat[0] = 0.1;
//                pushAction(a);
//            }
            if (action.argEntity[1]->historyPosition.size() >= 2 &&
                (action.argEntity[1]->historyPosition[1].second.len(action.argEntity[1]->historyPosition[0].second)) /
                action.argEntity[1]->historyPosition[1].first.elapsed(action.argEntity[1]->historyPosition[0].first) < 0.1) {
                Action a(action.argEntity[1], Timer::getNow(), ENTITY_SLIDE_VELOCITY_DISTANCE);
                a.argCoord[0] = move;
                a.argFloat[0] = 0.1;
                pushAction(a);
            }
            break;
        }
        case GLOBAL_DESTROY:
            destroyEntity(action.argEntity[0]->id);
            break;
        default:
            debug << "Error: unknown command for global action\n";
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

void Game::pushAction(const Action &action) {
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
            id = "egg$" + randomId();
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
//    debug << "Destroying " << id << " with " << result->second.use_count() << " uses\n";
    result->second->deleted = true;
    neighborsFinder.destroyEntry(result->second);
    entities.erase(result);
//    delete result->second.get();
}

void Game::render() {
    for (auto entity: entities) {
        entity.second->pushQuads();
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

    // for (auto entity : entities) {
    //     Graphics::drawText(entity.first, sf::Color::Black, 14, Camera::getScreenPos(entity.second->position) + UIVec(0., 30.), .5, sf::Color(255, 255, 255, 100), 3.);
    // }
    if (controls.facingEntity) {
        Graphics::drawText(controls.facingEntity->getDescriptionStr(), sf::Color::Black, 12., Camera::getScreenPos(controls.facingEntity->position) + UIVec(0., 40.), 0.,
                           sf::Color(255, 255, 255, 140), 3.);
        Graphics::insertUserWireframe(
                Camera::getScreenPos(controls.facingEntity->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.0 * PI) +
                UIVec(0, -controls.facingEntity->zPosition * Camera::getScale()),
                Camera::getScreenPos(controls.facingEntity->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 0.5 * PI) +
                UIVec(0, -controls.facingEntity->zPosition * Camera::getScale()),
                Camera::getScreenPos(controls.facingEntity->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.0 * PI) +
                UIVec(0, -controls.facingEntity->zPosition * Camera::getScale()),
                Camera::getScreenPos(controls.facingEntity->position) + Camera::getAngleVector(.3, Timer::getGlobalStart().elapsed() * -2. * PI + 1.5 * PI) +
                UIVec(0, -controls.facingEntity->zPosition * Camera::getScale()),
                sf::Color(255, 150, 60, 100), sf::Color(0, 0, 0, 100)
        );
    }
}

static constexpr const char* defaultFilePath = ".\\save.ya";

void Game::save() {
    std::ofstream fout(defaultFilePath);
    if (!fout.is_open()) std::cerr << "file saving failed";

    fout << Serialization::serialize<Game>(*this);
    if (fout.bad()) std::cerr << "file saving failed";
    fout.close();
}

void Game::load(const char* filepath) {
    std::ifstream fin(filepath);
    if (!fin.is_open()) std::cerr << "file loading failed";

    std::string str;
    fin >> str;
    Serialization::deserialize(*this, str);
    if (fin.bad()) std::cerr << "file loading failed";

    for (auto &e: entities) {
        switch (e.second->type) {
            case PLAYER: {
                std::shared_ptr<Player> tmp(new Player(this, e.second));
                e.second.reset();
                e.second = tmp;
                player = tmp;
                break;
            }
            case DUCK: {
                std::shared_ptr<Duck> tmp(new Duck(this, e.second));
                e.second.reset();
                e.second = tmp;
                break;
            }
        }
        pushAction(Action(e.second, Timer::getNow(), ON_CREATION));
    }

    fin.close();
}

void Game::setPlayer(std::shared_ptr<Entity> &player) {
    this->player = player;
}
