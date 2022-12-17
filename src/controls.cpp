#include "controls.h"
#include "game.h"
#include "player.h"
#include "duck.h"
#include "egg.h"
#include "eggcarton.h"
#include "audio.h"

Controls::Controls(Game* game) :
        game(game) {
}

void Controls::update() {
    facingEntity = getFacingEntity();
    facingTile = &getFacingTile();

    leftMouseClickAction = UNDEFINED;
    leftMouseAltClickAction = UNDEFINED;
    rightMouseClickAction = UNDEFINED;
    rightMouseAltClickAction = UNDEFINED;

    for (int hand = Player::InventorySlots::LEFT_HAND; hand <= Player::InventorySlots::RIGHT_HAND; hand++) {
        ControlsActions clickAction = UNDEFINED;
        ControlsActions altClickAction = UNDEFINED;
        auto inv = game->player->inventory[hand];

        if (inv == nullptr) { // hand empty
            if (facingEntity == nullptr) {
                clickAction = NONE_NO_ITEM_SELECTED;
                altClickAction = NONE_NO_ITEM_SELECTED;
            }
            else {
                switch (facingEntity->type) {
                    case DUCK:
                    case EGG:
                        clickAction = PICK_UP_ITEM;
                        break;
                    case EGG_CARTON:
                        clickAction = PICK_UP_CONTAINER;
                        // alt click picks up contents
                        int closestSlot = -1;
                        for (int slot = EggCarton::InventorySlots::EGG_0; slot <= EggCarton::InventorySlots::EGG_9; slot++) {
                            if (facingEntity->inventory[slot] == nullptr)
                                continue;
                            if (closestSlot == -1 || Camera::getMouseCoord().len(facingEntity->inventoryPosition[slot]) <
                                                     Camera::getMouseCoord().len(
                                                             facingEntity->inventoryPosition[closestSlot])) // should I use on screen closest or ingame coord closest?
                                closestSlot = slot;

                        }
                        if (closestSlot != -1) {
                            altClickAction = PICK_UP_ITEM_FROM_CONTAINER;
                            Graphics::insertUserWireframe(
                                    Camera::getScreenPos(facingEntity->inventoryPosition[closestSlot]) +
                                    Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 0.0 * PI),
                                    Camera::getScreenPos(facingEntity->inventoryPosition[closestSlot]) +
                                    Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 0.5 * PI),
                                    Camera::getScreenPos(facingEntity->inventoryPosition[closestSlot]) +
                                    Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 1.0 * PI),
                                    Camera::getScreenPos(facingEntity->inventoryPosition[closestSlot]) +
                                    Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 1.5 * PI),
                                    sf::Color(100, 255, 100, 100), sf::Color(0, 0, 0, 100)
                            );
                        }
                        break;
                }
            }
        }
        else {
            switch (inv->type) {
                case DUCK:
                case EGG:
                    clickAction = DROP_ITEM;
                    break;
                case EGG_CARTON:
                    clickAction = DROP_CONTAINER;
                    break;
            }
            if (facingEntity != nullptr) {
                switch (facingEntity->type) {
                    case DUCK:
                    case EGG:
                        break;
                    case EGG_CARTON:
                        switch (inv->type) {
                            case EGG: {
                                // alt click stores item
                                int closestSlot = -1;
                                for (int slot = EggCarton::InventorySlots::EGG_0; slot <= EggCarton::InventorySlots::EGG_9; slot++) {
                                    if (facingEntity->inventory[slot] != nullptr)
                                        continue;
                                    if (closestSlot == -1 || Camera::getMouseCoord().len(facingEntity->inventoryPosition[slot]) <
                                                             Camera::getMouseCoord().len(
                                                                     facingEntity->inventoryPosition[closestSlot])) // should I use on screen closest or ingame coord closest?
                                        closestSlot = slot;

                                }
                                if (closestSlot != -1) {
                                    altClickAction = STORE_ITEM_TO_CONTAINER;
                                    Graphics::insertUserWireframe(
                                            Camera::getScreenPos(facingEntity->inventoryPosition[closestSlot]) +
                                            Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 0.0 * PI),
                                            Camera::getScreenPos(facingEntity->inventoryPosition[closestSlot]) +
                                            Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 0.5 * PI),
                                            Camera::getScreenPos(facingEntity->inventoryPosition[closestSlot]) +
                                            Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 1.0 * PI),
                                            Camera::getScreenPos(facingEntity->inventoryPosition[closestSlot]) +
                                            Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 1.5 * PI),
                                            sf::Color(100, 255, 100, 100), sf::Color(0, 0, 0, 100)
                                    );
                                }
                                else
                                    altClickAction = NONE_CONTAINER_FULL;
                                break;
                            }
                            default:
                                altClickAction = NONE_CONTAINER_MISMATCH;
                        }
                        break;
                }
            }
        }

        if (hand == Player::InventorySlots::LEFT_HAND) {
            leftMouseClickAction = clickAction;
            leftMouseAltClickAction = altClickAction;
        }
        if (hand == Player::InventorySlots::RIGHT_HAND) {
            rightMouseClickAction = clickAction;
            rightMouseAltClickAction = altClickAction;
        }
    }
}

std::shared_ptr<Entity> Controls::getFacingEntity(EntityType filter) {
    auto nearby = game->neighborsFinder.findNeighbors(game->player->position, 2., filter);
    std::shared_ptr<Entity> facingEntity = nullptr;
    double bestScore = 1e8;
    for (auto e: nearby) {
        if (e == game->player)
            continue;
        if (!e->selectable)
            continue;
        UIVec epos = Camera::getScreenPos(e->position);
        UIVec ppos = Camera::getScreenPos(game->player->position);
        double angle = std::abs(subtractAngle(game->player->position.angle(e->position), game->player->heading));
        double dist = epos.len(ppos);
        // debug << subtractAngle(0, 303.13);
        // debug << e->id << ": " << angle << "  " << dist << "\n";
        if (dist < 4. * Camera::getScale() && angle < 1.5) {
            if (angle * dist < bestScore) {
                facingEntity = e;
                bestScore = angle * dist;
            }
        }
    }
    return facingEntity;
}

Map::Tile &Controls::getFacingTile() {
    /// TODO: implement obstruction
    coord refPnt = game->player->position + (Camera::getMouseCoord() - game->player->position).unit() * std::min((Camera::getMouseCoord() - game->player->position).len(), 2.);
    Graphics::insertUserWireframe(
            Camera::getScreenPos(refPnt) + UIVec(3, 0),
            Camera::getScreenPos(refPnt) + UIVec(0, 3),
            Camera::getScreenPos(refPnt) + UIVec(-3, 0),
            Camera::getScreenPos(refPnt) + UIVec(0, -3),
            sf::Color(255, 255, 255, 255), sf::Color(0, 0, 0, 100)
    );
    std::pair<int, int> tile = {std::floor(refPnt.x), std::floor(refPnt.y)};
    Graphics::insertUserWireframe(
            Camera::getScreenPos(coord(tile.first, tile.second) + coord(0.1, 0.1)),
            Camera::getScreenPos(coord(tile.first, tile.second) + coord(0.9, 0.1)),
            Camera::getScreenPos(coord(tile.first, tile.second) + coord(0.9, 0.9)),
            Camera::getScreenPos(coord(tile.first, tile.second) + coord(0.1, 0.9)),
            sf::Color(100, 255, 255, 255), sf::Color(0, 0, 0, 100)
    );
    return game->map.getTile(tile.first, tile.second);
}


void Controls::handleKeyPress(sf::Event &event) {
    if (!Graphics::getRenderWindow().hasFocus())
        return; // let's stop assssdddddddasssssssss from happening

    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::F1:
                Graphics::showWireframe = !Graphics::showWireframe;
                debug << "Toggled wireframe: " << Graphics::showWireframe << "\n";
                break;
            case sf::Keyboard::F2:
                game->showCollisionBoxes = !game->showCollisionBoxes;
                debug << "Toggled collision boxes: " << game->showCollisionBoxes << "\n";
                break;
            case sf::Keyboard::F3:
                debugStream.str("");
                break;
            case sf::Keyboard::F4:
                Graphics::debugOutOfSight = !Graphics::debugOutOfSight;
                debug << "Toggled debug out of sight: " << Graphics::debugOutOfSight << "\n";
                break;
            case sf::Keyboard::F6:
                for (auto duck: game->entities) {
                    if (duck.second->type == DUCK) {
                        Action a(duck.second, Timer::getNow(), DUCK_DUCKWALK_TO_UNTIL);
                        a.argCoord[0].x = Camera::getMouseCoord().x + (getRand() - .5);
                        a.argCoord[0].y = Camera::getMouseCoord().y + (getRand() - .5);
                        a.argFloat[0] = 15.;
                        game->pushAction(a);
                    }
                }
                break;
            case sf::Keyboard::F7:
                for (auto &e: game->entities) {
                    if (e.second->type == EntityType::DUCK) {
                        Action a(Timer::getNow(), GLOBAL_DESTROY);
                        a.argEntity[0] = e.second;
                        game->pushAction(a);
                        //                game->destroyEntity(event.second->id);
                    }
                }
                break;
            case sf::Keyboard::F11:
                graphicsIsFullscreen = !graphicsIsFullscreen;
                Graphics::createWindow(graphicsIsFullscreen);
                break;

            case sf::Keyboard::W:
                dirPadPress |= 0b0001;
                break;
            case sf::Keyboard::S:
                dirPadPress |= 0b0010;
                break;
            case sf::Keyboard::A:
                dirPadPress |= 0b0100;
                break;
            case sf::Keyboard::D:
                dirPadPress |= 0b1000;
                break;
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {

            case sf::Keyboard::W:
                dirPadPress &= ~0b0001;
                break;
            case sf::Keyboard::S:
                dirPadPress &= ~0b0010;
                break;
            case sf::Keyboard::A:
                dirPadPress &= ~0b0100;
                break;
            case sf::Keyboard::D:
                dirPadPress &= ~0b1000;
                break;
        }
    }
}

void Controls::handleMousePress(sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        // no alt

        int hand = -1;
        ControlsActions action = UNDEFINED;
        if (event.mouseButton.button == sf::Mouse::Button::Left) {
            action = (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)) ? leftMouseAltClickAction : leftMouseClickAction;
            hand = Player::InventorySlots::LEFT_HAND;
        }
        if (event.mouseButton.button == sf::Mouse::Button::Right) {
            action = (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)) ? rightMouseAltClickAction : rightMouseClickAction;
            hand = Player::InventorySlots::RIGHT_HAND;
        }
        if (hand == -1)
            return;

        switch (action) {
            case PICK_UP_ITEM:
            case PICK_UP_CONTAINER: {
                Action a(facingEntity, Timer::getNow(), ENTITY_OWN_BY);
                a.argEntity[0] = game->player;
                a.argInt[0] = hand;
                game->pushAction(a);
                break;
            }
            case PICK_UP_ITEM_FROM_CONTAINER: {
                int closestSlot = -1;
                for (int slot = EggCarton::InventorySlots::EGG_0; slot <= EggCarton::InventorySlots::EGG_9; slot++) {
                    if (facingEntity->inventory[slot] == nullptr)
                        continue;
                    if (closestSlot == -1 || Camera::getMouseCoord().len(facingEntity->inventoryPosition[slot]) <
                                             Camera::getMouseCoord().len(
                                                     facingEntity->inventoryPosition[closestSlot])) // should I use on screen closest or ingame coord closest?
                        closestSlot = slot;

                }
                if (closestSlot != -1) {
                    game->pushAction(Action(facingEntity->inventory[closestSlot], Timer::getNow(), ENTITY_UNOWN));
                    {
                        Action a(facingEntity->inventory[closestSlot], Timer::getNow() + TIME_SMALL_INC, ENTITY_OWN_BY);
                        a.argEntity[0] = game->player; // player
                        a.argInt[0] = hand;
                        game->pushAction(a);
                    }
                }
                break;
            }
            case DROP_ITEM:
            case DROP_CONTAINER:
                game->pushAction(Action(game->player->inventory[hand], Timer::getNow(), ENTITY_UNOWN));
                break;
            case STORE_ITEM_TO_CONTAINER: {
                int closestSlot = -1;
                for (int slot = EggCarton::InventorySlots::EGG_0; slot <= EggCarton::InventorySlots::EGG_9; slot++) {
                    if (facingEntity->inventory[slot] != nullptr)
                        continue;
                    if (closestSlot == -1 || Camera::getMouseCoord().len(facingEntity->inventoryPosition[slot]) <
                                             Camera::getMouseCoord().len(
                                                     facingEntity->inventoryPosition[closestSlot])) // should I use on screen closest or ingame coord closest?
                        closestSlot = slot;

                }
                if (closestSlot != -1) {
                    game->pushAction(Action(game->player->inventory[hand], Timer::getNow(), ENTITY_UNOWN));
                    {
                        Action a(game->player->inventory[hand], Timer::getNow() + TIME_SMALL_INC, ENTITY_OWN_BY);
                        a.argEntity[0] = facingEntity; // the container
                        a.argInt[0] = closestSlot;
                        game->pushAction(a);
                    }
                }
                break;
            }
        }


//            if (event.mouseButton.button == sf::Mouse::Button::Left) {
//                if (facingEntity) {
//                    /// if a container is highlighted (for example an egg carton) -> store item
//                    if (facingEntity->type == EGG_CARTON && game->player->inventory[Player::InventorySlots::LEFT_HAND]->type == EGG) {
//                    }
//                }
//
////                if (game->player->inventory[Player::InventorySlots::LEFT_HAND] == nullptr && facingEntity) {
////                    Action a(facingEntity, Timer::getNow(), ENTITY_OWN_BY);
////                    a.argEntity[0] = game->player;
////                    a.argInt[0] = Player::InventorySlots::LEFT_HAND;
////                    game->pushAction(a);
////                } else if (game->player->inventory[Player::InventorySlots::LEFT_HAND] != nullptr) {
////                    game->pushAction(Action(game->player->inventory[Player::InventorySlots::LEFT_HAND], Timer::getNow(), ENTITY_UNOWN));
////                }
//            }
//
//            if (facingEntity == nullptr && facingTile != nullptr) {
//                facingTile->setTileType(Map::Tile::MOAI);
//            }
//        }
//        else {
//            if (event.mouseButton.button == sf::Mouse::Button::Left) {
//                if (game->player->inventory[Player::InventorySlots::LEFT_HAND] == nullptr && facingEntity) {
//                    /// when hands are empty -> pick up item
//                    Action a(facingEntity, Timer::getNow(), ENTITY_OWN_BY);
//                    a.argEntity[0] = game->player;
//                    a.argInt[0] = Player::InventorySlots::LEFT_HAND;
//                    game->pushAction(a);
//                }
//                else if (game->player->inventory[Player::InventorySlots::LEFT_HAND] != nullptr) {
//                    /// when holding an entity -> drop item
//                    game->pushAction(Action(game->player->inventory[Player::InventorySlots::LEFT_HAND], Timer::getNow(), ENTITY_UNOWN));
//                }
//            }
//            if (event.mouseButton.button == sf::Mouse::Button::Right) {
//                if (game->player->inventory[Player::InventorySlots::RIGHT_HAND] == nullptr && facingEntity) {
//                    /// when hands are empty -> pick up item
//                    Action a(facingEntity, Timer::getNow(), ENTITY_OWN_BY);
//                    a.argEntity[0] = game->player;
//                    a.argInt[0] = Player::InventorySlots::RIGHT_HAND;
//                    game->pushAction(a);
//                }
//                else if (game->player->inventory[Player::InventorySlots::RIGHT_HAND] != nullptr) {
//                    /// when holding an entity -> drop item
//                    game->pushAction(Action(game->player->inventory[Player::InventorySlots::RIGHT_HAND], Timer::getNow(), ENTITY_UNOWN));
//                }
//            }
    }
}

void Controls::handleSoundOnAction(sf::Event &event){
    if(event.type == sf::Event::KeyPressed){
        switch(event.key.code){
            case sf::Keyboard::W:
                Audio::playSound("walk.wav"); // No continuous walking sound QAQ
            case sf::Keyboard::A:
                Audio::playSound("walk.wav");
            case sf::Keyboard::S:
                Audio::playSound("walk.wav");
            case sf::Keyboard::D:
                Audio::playSound("walk.wav");
        }
    }

    if (event.type == sf::Event::MouseButtonPressed){
        Audio::playSound("tick.wav");
    }
}

