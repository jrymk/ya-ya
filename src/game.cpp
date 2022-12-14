#include "game.h"
#include <iomanip>
#include <thread>
#include "player.h"
#include "duck.h"
#include "egg.h"
#include "eggcarton.h"
#include "truck.h"
#include "NPC.h"
#include "ui.h"
#include "serializationExtended.h"

Game::Game() :
        controller(this),
        neighborsFinder(this),
        controls(this),
        ui(this) {}

void Game::update() {
    if (controller.gameState == GameController::TITLE_SCREEN) return;  // no update on title
    Timer updateTimer;

    // control player movement
    UIVec moveVec; // on screen vec
    if (controls.dirPadPress & 0b0001)
        moveVec.y -= 1.;
    if (controls.dirPadPress & 0b0010)
        moveVec.y += 1.;
    if (controls.dirPadPress & 0b0100)
        moveVec.x -= 1.;
    if (controls.dirPadPress & 0b1000)
        moveVec.x += 1.;
    if (moveVec.len(UIVec()) > .1)
        player->slideVelocity = coord(Camera::getTransform().getInverse().transformPoint((moveVec).getVec2f())).unit() * 4.;
    else
        player->slideVelocity = coord();

    player->heading = player->position.angle(Camera::getMouseCoord());
    addRandomDuck();

    neighborsFinder.update();
    mapUpdate();

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

void Game::mapUpdate() {

//    double minX = Camera::getCoord(Camera::getViewport().pos + Camera::getViewport().size * UIVec(0., 1.)).x;
//    double maxX = Camera::getCoord(Camera::getViewport().pos + Camera::getViewport().size * UIVec(1., 0.)).x;
//    double minY = Camera::getCoord(Camera::getViewport().pos + Camera::getViewport().size * UIVec(1., 1.)).y;
//    double maxY = Camera::getCoord(Camera::getViewport().pos + Camera::getViewport().size * UIVec(0., 0.)).y;
//
//    for (int x = std::floor(minX); x <= std::floor(maxX + 1.); x++) {
//        for (int y = std::floor(minY); y <= std::floor(maxY + 1.); y++) {
////            map.getTile(x, y).update();
//        }
//    }
}

void Game::processCollisions() {
    for (auto eit: entities) {
        auto &e = eit.second;
        // entity collision
        if (!e->collisionCollidable)
            continue;
        auto result = neighborsFinder.findNeighbors(e->underlyingPos, 5.);
        for (auto f: result) {
            if (e == f)
                continue;
            if (!f->collisionPushable)
                continue;

//            if (f->collideBox.collide(e->collideBox, f->position, e->position).len() > 0.01) {
//                if (!f->collisionPushable)
//                    continue;
//                if (e->ownedBy == f || f->ownedBy == e)
//                    continue;
//                if (f->historyPosition.size() >= 2 &&
//                    (f->historyPosition[1].second.len(f->historyPosition[0].second)) / f->historyPosition[1].first.elapsed(f->historyPosition[0].first) < 0.12) {
//                    Action a(f, Timer::getNow(), ENTITY_SLIDE_VELOCITY_DISTANCE);
//                    a.argCoord[0] = f->collideBox.collide(e->collideBox, f->position, e->position) *
//                                    std::pow(f->collideBox.collide(e->collideBox, f->position, e->position).len(), -3);
//                    a.argFloat[0] = std::min(std::pow(f->collideBox.collide(e->collideBox, f->position, e->position).len(), 2), 0.2);
//                    pushAction(a); /// TODO: fix collision
//                }

            if (e->position.len(f->position) < .3) {
                if (!f->collisionPushable)
                    continue;
                if (e->ownedBy == f || f->ownedBy == e)
                    continue;
                coord delta = f->position - e->position;
                if (delta.len() < .00001)
                    delta = coord::getRandCoord();
                coord move = delta / delta.len() / delta.len() / 3.;
                if (f->historyPosition.size() >= 2 &&
                    (f->historyPosition[1].second.len(f->historyPosition[0].second)) / f->historyPosition[1].first.elapsed(f->historyPosition[0].first) < 0.1) {
                    Action a(f, Timer::getNow(), ENTITY_SLIDE_VELOCITY_DISTANCE);
                    a.argCoord[0] = move;
                    a.argFloat[0] = 0.1;
                    pushAction(a); /// TODO: use entity collision boxes as entity collision ref (if it ain't broke don't fix it)
                }
            }
        }
    }
    for (auto eit: entities) {
        auto &e = eit.second;
        if (e->collisionNoEnv)
            continue;
        // map collision
        for (int tx = int(std::round(e->underlyingPos.x - e->footprint.x / 2 - 1.)); tx <= int(std::round(e->underlyingPos.x + e->footprint.x / 2)); tx++) {
            for (int ty = int(std::round(e->underlyingPos.y - e->footprint.y / 2 - 1.)); ty <= int(std::round(e->underlyingPos.y + e->footprint.y / 2)); ty++) {
                for (auto &cb: map.getTile(tx, ty).collideBoxes) {
                    if (!e->collisionPushable)
                        continue;
                    e->underlyingPos = e->underlyingPos + e->collideBox.collide(cb, e->underlyingPos, coord(tx, ty));
//                    e->position = e->underlyingPos;
                    if (showCollisionBoxes)
                        Graphics::insertUserWireframe(Camera::getScreenPos(coord(tx, ty) + cb.center + coord(-cb.size.x / 2., -cb.size.y / 2.)),
                                                      Camera::getScreenPos(coord(tx, ty) + cb.center + coord(cb.size.x / 2., -cb.size.y / 2.)),
                                                      Camera::getScreenPos(coord(tx, ty) + cb.center + coord(cb.size.x / 2., cb.size.y / 2.)),
                                                      Camera::getScreenPos(coord(tx, ty) + cb.center + coord(-cb.size.x / 2., cb.size.y / 2.)),
                                                      (cb.isCircle ? sf::Color::Magenta : sf::Color::Blue), sf::Color::Transparent);
                }
            }
        }
        if (showCollisionBoxes)
            Graphics::insertUserWireframe(Camera::getScreenPos(e->position + e->collideBox.center + coord(-e->collideBox.size.x / 2., -e->collideBox.size.y / 2.)),
                                          Camera::getScreenPos(e->position + e->collideBox.center + coord(e->collideBox.size.x / 2., -e->collideBox.size.y / 2.)),
                                          Camera::getScreenPos(e->position + e->collideBox.center + coord(e->collideBox.size.x / 2., e->collideBox.size.y / 2.)),
                                          Camera::getScreenPos(e->position + e->collideBox.center + coord(-e->collideBox.size.x / 2., e->collideBox.size.y / 2.)),
                                          (e->collideBox.isCircle ? sf::Color::Magenta : sf::Color::Blue), sf::Color::Transparent);
    }

//    for (auto eit: entities) {
//        auto &e = eit.second;
//        if (e->collisionNoEnv)
//            continue;
//        // map collision
//        for (int tx = int(std::round(e->position.x - e->footprint.x / 2 - 1.)); tx <= int(std::round(e->position.x + e->footprint.x / 2)); tx++) {
//            for (int ty = int(std::round(e->position.y - e->footprint.y / 2 - 1.)); ty <= int(std::round(e->position.y + e->footprint.y / 2)); ty++) {
//                for (auto &cb: map.getTile(tx, ty).collideBoxes) {
//                    e->position = e->position + e->collideBox.collide(cb, e->position, coord(tx, ty));
////                    e->position = e->underlyingPos;
//                    if (showCollisionBoxes)
//                        Graphics::insertUserWireframe(Camera::getScreenPos(coord(tx, ty) + cb.center + coord(-cb.size.x / 2., -cb.size.y / 2.)),
//                                                      Camera::getScreenPos(coord(tx, ty) + cb.center + coord(cb.size.x / 2., -cb.size.y / 2.)),
//                                                      Camera::getScreenPos(coord(tx, ty) + cb.center + coord(cb.size.x / 2., cb.size.y / 2.)),
//                                                      Camera::getScreenPos(coord(tx, ty) + cb.center + coord(-cb.size.x / 2., cb.size.y / 2.)),
//                                                      (cb.isCircle ? sf::Color::Magenta : sf::Color::Blue), sf::Color::Transparent);
//                }
//            }
//        }
//        if (showCollisionBoxes)
//            Graphics::insertUserWireframe(Camera::getScreenPos(e->position + e->collideBox.center + coord(-e->collideBox.size.x / 2., -e->collideBox.size.y / 2.)),
//                                          Camera::getScreenPos(e->position + e->collideBox.center + coord(e->collideBox.size.x / 2., -e->collideBox.size.y / 2.)),
//                                          Camera::getScreenPos(e->position + e->collideBox.center + coord(e->collideBox.size.x / 2., e->collideBox.size.y / 2.)),
//                                          Camera::getScreenPos(e->position + e->collideBox.center + coord(-e->collideBox.size.x / 2., e->collideBox.size.y / 2.)),
//                                          (e->collideBox.isCircle ? sf::Color::Magenta : sf::Color::Blue), sf::Color::Transparent);
//    }
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
        }
        else
            break;
    }
    for (auto &action: followUpActions)
        if (action.isGlobal || !action.entity->deleted)
            actionList.push(action); // will run next update
}

void Game::runAction(Action &action, std::vector<Action> &followUpActions) {
    if (!action.isGlobal) {
        if (!action.entity->deleted)
            action.entity->runActionEntity(action, followUpActions); // first go through entity
        return;
    }
    // global actions
    switch (action.command) {
        case GLOBAL_DESTROY:
            if (action.argEntity[0]->ownedBy != nullptr)
                action.argEntity[0]->ownedBy->inventory[action.argEntity[0]->ownedSlot] = nullptr;
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
        case EGG_CARTON:
            id = "eggcarton$" + randomId();
            break;
        case TRUCK:
            id = "truck$" + randomId();
            break;
        case NPC:
            id = "npc$" + randomId();
            break;
        default:
            debug << "hey you forgot me\n";
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
    if (controller.gameState == GameController::TITLE_SCREEN) return;  // no render on title
    renderMap();

    for (auto entity: entities) {
        entity.second->pushQuads();
    }

    // player target location indicator
    Graphics::insertUserWireframe(
            Camera::getScreenPos(player->underlyingPos) + UIVec(5, 0),
            Camera::getScreenPos(player->underlyingPos) + UIVec(0, 5),
            Camera::getScreenPos(player->underlyingPos) + UIVec(-5, 0),
            Camera::getScreenPos(player->underlyingPos) + UIVec(0, -5),
            sf::Color(100, 255, 255, int(std::max(std::min((player->underlyingPos.len(player->position) - .5) * 255., 255.), 0.))),
            sf::Color(0, 0, 0, int(std::max(std::min((player->underlyingPos.len(player->position) - .5) * 255., 255.), 0.)))
    );
    Graphics::insertUserWireframe(
            Camera::getScreenPos(player->underlyingPos) + UIVec(3, 0),
            Camera::getScreenPos(player->underlyingPos) + UIVec(0, 3),
            Camera::getScreenPos(player->underlyingPos) + UIVec(-3, 0),
            Camera::getScreenPos(player->underlyingPos) + UIVec(0, -3),
            sf::Color(0, 0, 0, int(std::max(std::min((player->underlyingPos.len(player->position) - .5) * 255., 255.), 0.))),
            sf::Color(100, 255, 255, int(std::max(std::min((player->underlyingPos.len(player->position) - .5) * 255., 255.), 0.)))
    );
}

void Game::renderMap() {
    double minX = Camera::getCoord(Camera::getViewport().pos + Camera::getViewport().size * UIVec(0., 1.)).x;
    double maxX = Camera::getCoord(Camera::getViewport().pos + Camera::getViewport().size * UIVec(1., 0.)).x;
    double minY = Camera::getCoord(Camera::getViewport().pos + Camera::getViewport().size * UIVec(1., 1.)).y;
    double maxY = Camera::getCoord(Camera::getViewport().pos + Camera::getViewport().size * UIVec(0., 0.)).y;

    // trial noob filter xD
    double timeUntilEnd = DAY_LENGTH - controller.startOfDayTp.elapsed();
    double duration = 7.;
    double offset = 3.;
    sf::Color beforeColor(255, 255, 255, 255);
    sf::Color targetColor(255, 178, 77, 255);
    if (timeUntilEnd < 3.) {
        duration = 3.;
        offset = 0.;
        beforeColor = targetColor;
        targetColor = sf::Color(0, 24, 79, 255);
    }
    sf::Color nowColor(beforeColor.r - (beforeColor.r - targetColor.r) * (1. - cap((timeUntilEnd - offset) / duration)),
                       beforeColor.g - (beforeColor.g - targetColor.g) * (1. - cap((timeUntilEnd - offset) / duration)),
                       beforeColor.b - (beforeColor.b - targetColor.b) * (1. - cap((timeUntilEnd - offset) / duration)),
                       beforeColor.a - (beforeColor.a - targetColor.a) * (1. - cap((timeUntilEnd - offset) / duration)));

    Graphics::globalShadingColor = nowColor;

    for (int x = std::floor(minX); x <= std::floor(maxX + 1.); x++) {
        for (int y = std::floor(minY); y <= std::floor(maxY + 1.); y++) {
            map.getTile(x, y).pushQuads(0., Graphics::globalShadingColor, Graphics::globalShadingColor);
        }
    }

//    for (int x = int(minX) & ~0b1111; x <= (int(maxX + 1.) & ~0b1111) + 0b1111; x += 0b1) {
//        for (int y = int(minY) & ~0b1111; y <= (int(maxY + 1.) & ~0b1111) + 0b1111; y += 0b1) {
//            map.getTile(0, 0).pushQuads(x, y);
//        }
//    }
}

static constexpr const char* defaultFilePath = ".\\save.ya";

void Game::save() {
    while (actionList.size()) {  // process action list
        actionSaveList.emplace_back(actionList.top());
        actionList.pop();
    }

    Timer timerForSave;
    for (Action &a: actionSaveList) a.time.saveTimer(timerForSave);

    std::ofstream fout(defaultFilePath);
    if (!fout.is_open()) std::cerr << "file saving failed";

    fout << Serialization::serialize<Game>(*this);
    if (fout.bad()) std::cerr << "file saving failed";
    fout.close();
}

void Game::load(const char* filepath) {
    entities.clear();
    while (!actionList.empty()) actionList.pop();
    std::ifstream fin(filepath);
    if (!fin.is_open()) std::cerr << "file loading failed";

    std::string str;
    std::getline(fin, str);
    Serialization::deserialize(*this, str);
    if (fin.bad()) std::cerr << "file loading failed";
    debug << "deserialization done\n";

    // setting game*
    controller.setGame(this);
    for (auto &e: entities) {  // process entities (reminder: set serializationExtended)
        switch (e.second->type) {
            case PLAYER: {
                std::dynamic_pointer_cast<Player>(e.second)->game = this;
                break;
            }
            case DUCK: {
                std::dynamic_pointer_cast<Duck>(e.second)->game = this;
                // pushAction(Action(e.second, Timer::getNow(), ON_CREATION));
                break;
            }
            case EGG: {
                std::dynamic_pointer_cast<Egg>(e.second)->game = this;
                // pushAction(Action(e.second, Timer::getNow(), ON_CREATION));
                break;
            }
            case EGG_CARTON: {
                std::dynamic_pointer_cast<EggCarton>(e.second)->game = this;
                // pushAction(Action(e.second, Timer::getNow(), ON_CREATION));
                break;
            }
            case TRUCK: {
                std::dynamic_pointer_cast<Truck>(e.second)->game = this;
                // pushAction(Action(e.second, Timer::getNow(), ON_CREATION));
                break;
            }
        }
    }

    Timer timerForLoad;
    for (Action &a: actionSaveList) {  // process action list
        a.time.loadTimer(timerForLoad);
        actionList.emplace(a);
    }
    actionSaveList.clear();

    for (auto &p: map.tileData) {  // process map
        p.second.setMap(&map);
        for (auto &t: p.second.tiles)
            t.setMap(&map);
    }

    SaveUtilities::clearObjTracker();  // don't forget to clear smart ptr ownership here
    fin.close();
    debug << "game successfully loaded\n";
}

void Game::setPlayer(std::shared_ptr<Player> &player) {
    this->player = player;
}

void Game::initTruckCollisionBoxes(int baseX, int baseY) {
    // even loading from file needs this because we don't have map serialization and deserialization, right?
    for (int x = baseX; x < 10; x++) {  // truck collision
        for (int y = baseY; y < 2; y++)
            map.getTile(x, y).setTileType(Map::Tile::TRUCK); // for collision, because I don't want to make collision boxes work with entities
    }
}

void Game::setTruck(std::shared_ptr<Truck> &truck) {
    this->truck = truck;
}

void Game::addRandomDuck() {
    if (getRand() > 1 / 900.) return;  // a duck most averagely dies after 300 secs, natural spawn rate 15 secs

    std::shared_ptr<Egg> egg = std::make_shared<Egg>(this);
    egg->id = newId(EGG);
    egg->opacity = 1.;

    double radius = 20. + 20. * getRand(), theta = 2. * M_PI * getRand();
    egg->underlyingPos.x = player->underlyingPos.x + radius * cos(theta);
    egg->underlyingPos.y = player->underlyingPos.y + radius * sin(theta);

    egg->genderIsMale = (getRand() > 0.5);
    egg->fertilized = true;
    insertEntity(egg);
}
