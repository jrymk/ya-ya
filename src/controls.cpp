#include "controls.h"
#include "game.h"
#include "player.h"
#include "duck.h"
#include "egg.h"
#include "eggcarton.h"
#include "model.h"
#include "audio.h"
#include "truck.h"

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
        int otherHand = 1 - hand;
        ControlsActions clickAction = UNDEFINED;
        ControlsActions altClickAction = UNDEFINED;
        auto inv = game->player->inventory[hand];
        auto otherInv = game->player->inventory[otherHand];

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
                        // alt click to pick up from container
                        if (tryPickUpFromContainer(facingEntity) >= 0)
                            altClickAction = PICK_UP_ITEM_FROM_FACING_CONTAINER;
                        break;
                    case TRUCK:
                        // alt click to pick up from container
                        if (tryPickUpFromContainer(facingEntity) >= 0)
                            altClickAction = PICK_UP_ITEM_FROM_FACING_CONTAINER;
                }
            }
        }
        else { // hand with stuff
            switch (inv->type) {
                case DUCK:
                    clickAction = DROP_ITEM;
                    break;
                case EGG:
                    clickAction = DROP_ITEM;
                    // egg -> egg carton
                    if (facingEntity && facingEntity->type == EGG_CARTON) {
                        int slot = tryStoreToContainer(facingEntity, inv);
                        if (slot == -2)
                            altClickAction = NONE_CONTAINER_FULL;
                        else if (slot >= 0)
                            altClickAction = STORE_ITEM_TO_FACING_CONTAINER;
                    }
                    // egg -> egg carton (other hand)
                    if (otherInv && otherInv->type == EGG_CARTON) {
                        int slot = tryStoreToContainer(otherInv, inv, true);
                        if (slot == -2)
                            altClickAction = NONE_CONTAINER_FULL;
                        else if (slot >= 0)
                            altClickAction = STORE_ITEM_TO_OTHER_HAND_CONTAINER;
                    }
                    break;
                case EGG_CARTON:
                    clickAction = DROP_CONTAINER;
                    // egg carton <- egg
                    if (facingEntity && facingEntity->type == EGG && otherInv == nullptr) { // other hand is empty
                        int slot = tryStoreToContainer(inv, facingEntity, true);
                        if (slot >= 0)
                            altClickAction = STORE_FACING_ITEM_TO_CONTAINER;
                    }
                    // egg carton -> truck
                    if (facingEntity && facingEntity->type == TRUCK) {
                        int slot = tryStoreToContainer(facingEntity, inv);
                        if (slot == -2)
                            altClickAction = NONE_CONTAINER_FULL;
                        else if (slot >= 0)
                            altClickAction = STORE_ITEM_TO_FACING_CONTAINER;
                    }
                    break;
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
    auto nearby = game->neighborsFinder.findNeighbors(game->player->position, 4., filter);
    std::shared_ptr<Entity> facingEntity = nullptr;
    double bestScore = 1e8;
    for (auto e: nearby) {
        if (e == game->player)
            continue;
        if (!e->selectable)
            continue;
//        coord epos = e->position;
//        coord ppos = game->player->position;
//        double angle = std::abs(subtractAngle(game->player->position.angle(e->position), game->player->heading));
//        double dist = epos.len(ppos);
//        // debug << subtractAngle(0, 303.13);
//        // debug << e->id << ": " << angle << "  " << dist << "\n";
//        if (dist < 3. && angle < 1.5) {
//            if (angle * dist < bestScore) {
//                facingEntity = e;
//                bestScore = angle * dist;
//            }
//        }

        coord c = Camera::getMouseCoord();
        if (std::abs(c.x - e->position.x) < e->footprint.x / 2. + 1. && std::abs(c.y - e->position.y) < e->footprint.y / 2. + 1.) {
            if (!facingEntity ||
                ((coord(std::max(0., std::abs(c.x - e->position.x) - e->footprint.x / 2. - 0.), std::max(0., std::abs(c.y - e->position.y) - e->footprint.y / 2. - 0.)).len()) <
                 bestScore)) {
                bestScore = (coord(std::max(0., std::abs(c.x - e->position.x) - e->footprint.x / 2. - 0.),
                                   std::max(0., std::abs(c.y - e->position.y) - e->footprint.y / 2. - 0.)).len());
                facingEntity = e;
            }
        }
    }
    if (facingEntity) {
        double dist = (coord(std::max(0., std::abs(game->player->position.x - facingEntity->position.x) - facingEntity->footprint.x / 2. - 0.),
                             std::max(0., std::abs(game->player->position.y - facingEntity->position.y) - facingEntity->footprint.y / 2. - 0.)).len());
        bool inReach = dist < 1.;

        if (dist < 4.) { // don't even render the indicators if TOO far
            for (int corner = 0; corner < 4; corner++) {
                auto quad = modelSelector[corner];
                float speed = 4.; // 2hz
                coord pulse(.05 * std::sin(Timer::getGlobalStart().elapsed() * speed * PI), .05 * std::sin(Timer::getGlobalStart().elapsed() * speed * PI));
//            quad.v0 = coord::getAngleVec(quad.v0.len(), facingEntity->heading + quad.v0.angle(UIVec(1., 0.))).getUIVec();
//            quad.v1 = coord::getAngleVec(quad.v1.len(), facingEntity->heading + quad.v1.angle(UIVec(1., 0.))).getUIVec();
//            quad.v2 = coord::getAngleVec(quad.v2.len(), facingEntity->heading + quad.v2.angle(UIVec(1., 0.))).getUIVec();
//            quad.v3 = coord::getAngleVec(quad.v3.len(), facingEntity->heading + quad.v3.angle(UIVec(1., 0.))).getUIVec();

                quad.v0 = Camera::getScreenPos(
                        coord(quad.v0.x, quad.v0.y) + facingEntity->position + facingEntity->collideBox.center +
                        (facingEntity->collideBox.size + pulse) * coord((corner & 0b1) ? .5 : -.5, (corner & 0b10) ? .5 : -.5));
                quad.v1 = Camera::getScreenPos(
                        coord(quad.v1.x, quad.v1.y) + facingEntity->position + facingEntity->collideBox.center +
                        (facingEntity->collideBox.size + pulse) * coord((corner & 0b1) ? .5 : -.5, (corner & 0b10) ? .5 : -.5));
                quad.v2 = Camera::getScreenPos(
                        coord(quad.v2.x, quad.v2.y) + facingEntity->position + facingEntity->collideBox.center +
                        (facingEntity->collideBox.size + pulse) * coord((corner & 0b1) ? .5 : -.5, (corner & 0b10) ? .5 : -.5));
                quad.v3 = Camera::getScreenPos(
                        coord(quad.v3.x, quad.v3.y) + facingEntity->position + facingEntity->collideBox.center +
                        (facingEntity->collideBox.size + pulse) * coord((corner & 0b1) ? .5 : -.5, (corner & 0b10) ? .5 : -.5));

                quad.c0 = inReach ? sf::Color(173, 240, 255, 255) : sf::Color(217, 217, 217, 255);
                quad.c1 = inReach ? sf::Color(173, 240, 255, 255) : sf::Color(217, 217, 217, 255);
                quad.c2 = inReach ? sf::Color(173, 240, 255, 255) : sf::Color(217, 217, 217, 255);
                quad.c3 = inReach ? sf::Color(173, 240, 255, 255) : sf::Color(217, 217, 217, 255);
                Graphics::insertQuad(quad);

            }
        }
        if (!inReach)
            facingEntity = nullptr;
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
        int otherHand = 1 - hand;

        /// we TRUST the results from controls.update, we TRUST the actions they gave is legal. Look how clean this code is
        switch (action) {
            case PICK_UP_ITEM:
            case PICK_UP_CONTAINER:
                changeOwner(facingEntity, game->player, hand);
                break;
            case PICK_UP_ITEM_FROM_FACING_CONTAINER:
                changeOwner(facingEntity->inventory[tryPickUpFromContainer(facingEntity)], game->player, hand);
                break;
            case DROP_ITEM:
            case DROP_CONTAINER:
                changeOwner(game->player->inventory[hand]);
                break;
            case STORE_ITEM_TO_FACING_CONTAINER:
                changeOwner(game->player->inventory[hand], facingEntity,
                            tryStoreToContainer(facingEntity, game->player->inventory[hand]));
                break;
            case STORE_ITEM_TO_OTHER_HAND_CONTAINER:
                changeOwner(game->player->inventory[hand], game->player->inventory[otherHand],
                            tryStoreToContainer(game->player->inventory[otherHand], game->player->inventory[hand], true));
                break;
            case STORE_FACING_ITEM_TO_CONTAINER:
                changeOwner(facingEntity, game->player->inventory[hand],
                            tryStoreToContainer(game->player->inventory[hand], facingEntity, true));
                break;

        }
    }
}

int Controls::tryStoreToContainer(const std::shared_ptr<Entity> &container, const std::shared_ptr<Entity> &item, bool useRandomSlot) {
    int slotRangeLo = -1, slotRangeHi = -1;
    switch (container->type) {
        case EGG_CARTON:
            switch (item->type) {
                case EGG:
                    slotRangeLo = EggCarton::InventorySlots::EGG_0;
                    slotRangeHi = EggCarton::InventorySlots::EGG_9;
                    break;
            }
            break;
        case TRUCK:
            switch (item->type) {
                case EGG_CARTON:
                    slotRangeLo = Truck::InventorySlots::TRUCK_A0;
                    slotRangeHi = Truck::InventorySlots::TRUCK_D3;
                    break;
            }
            break;
    }
    if (slotRangeLo == -1)
        return -1;

    std::vector<int> candidates;
    int closestSlot = -1;
    for (int slot = slotRangeLo; slot <= slotRangeHi; slot++) {
        if (container->inventory[slot] != nullptr)
            continue;
        candidates.push_back(slot);
        if (closestSlot == -1 || Camera::getMousePos().len(Camera::getScreenPos(container->inventoryPosition[slot].first, container->inventoryPosition[slot].second)) <
                                 Camera::getMousePos().len(Camera::getScreenPos(container->inventoryPosition[closestSlot].first,
                                                                                container->inventoryPosition[closestSlot].second))) // should I use on screen closest or ingame coord closest?
            closestSlot = slot;
    }
    if (useRandomSlot && candidates.size() > 0)
        return int(double(candidates.size()) * getRand());
    if (closestSlot != -1) {
        auto quad = modelEmptySlotSelector;
        quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) + container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second);
        quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) + container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second);
        quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) + container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second);
        quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) + container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second);
        quad.zDepth += (Camera::getScreenPos(container->inventoryPosition[closestSlot].first).y / Camera::getViewport().size.y - 0.5) / 100. + .1;
        quad.c0 = quad.c1 = quad.c2 = quad.c3 = sf::Color(255, 100, 100, 170 + 50 * std::sin(Timer::getGlobalStart().elapsed() * PI * 3));

        Graphics::insertQuad(quad);


//        Graphics::insertUserWireframe(
//                Camera::getScreenPos(container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second) +
//                Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 0.0 * PI),
//                Camera::getScreenPos(container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second) +
//                Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 0.5 * PI),
//                Camera::getScreenPos(container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second) +
//                Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 1.0 * PI),
//                Camera::getScreenPos(container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second) +
//                Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 1.5 * PI),
//                sf::Color(100, 255, 100, 100), sf::Color(0, 0, 0, 100)
//        );
        return closestSlot;
    }
    return -2;
}

int Controls::tryPickUpFromContainer(const std::shared_ptr<Entity> &container) {
    int slotRangeLo = -1, slotRangeHi = -1;
    switch (container->type) {
        case EGG_CARTON:
            slotRangeLo = EggCarton::InventorySlots::EGG_0;
            slotRangeHi = EggCarton::InventorySlots::EGG_9;
            break;
        case TRUCK:
            slotRangeLo = Truck::InventorySlots::TRUCK_A0;
            slotRangeHi = Truck::InventorySlots::TRUCK_D3;
            break;
    }
    if (slotRangeLo == -1)
        return -1;

    int closestSlot = -1;
    for (int slot = slotRangeLo; slot <= slotRangeHi; slot++) {
        if (container->inventory[slot] == nullptr)
            continue;
        if (closestSlot == -1 || Camera::getMousePos().len(Camera::getScreenPos(container->inventoryPosition[slot].first, container->inventoryPosition[slot].second)) <
                                 Camera::getMousePos().len(Camera::getScreenPos(container->inventoryPosition[closestSlot].first,
                                                                                container->inventoryPosition[closestSlot].second))) // should I use on screen closest or ingame coord closest?
            closestSlot = slot;
    }
    if (closestSlot != -1) {

        UIVec ospos = Camera::getScreenPos(container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second);
        Graphics::drawImage(imagaInventorySlotIndicator, ZDEPTH_ABOVE_GROUND + (ospos.y / Camera::getViewport().size.y - 0.5) / 100. + .1,
                            ospos + UIVec(0, -24 + 8 * std::sin(Timer::getGlobalStart().elapsed() * PI * 2)),
                            UIVec(.5, 1.), 1., sf::Color(255, 231, 122, 255));

//        Graphics::insertUserWireframe(
//                Camera::getScreenPos(container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second) +
//                Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 0.0 * PI),
//                Camera::getScreenPos(container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second) +
//                Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 0.5 * PI),
//                Camera::getScreenPos(container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second) +
//                Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 1.0 * PI),
//                Camera::getScreenPos(container->inventoryPosition[closestSlot].first, container->inventoryPosition[closestSlot].second) +
//                Camera::getAngleVector(.1, Timer::getGlobalStart().elapsed() * -4. * PI + 1.5 * PI),
//                sf::Color(100, 255, 100, 100), sf::Color(0, 0, 0, 100)
//        );
        return closestSlot;
    }
    return -2;
}

void Controls::changeOwner(const std::shared_ptr<Entity> &item, const std::shared_ptr<Entity> &newContainer, int newSlot) {
    if (item != nullptr)
        game->pushAction(Action(item, Timer::getNow(), ENTITY_UNOWN));
    if (newContainer == nullptr || newSlot < 0)
        return;
    Action a(item, Timer::getNow() + TIME_SMALL_INC, ENTITY_OWN_BY);
    a.argEntity[0] = newContainer;
    a.argInt[0] = newSlot;
    game->pushAction(a);
}

void Controls::handleSoundOnAction(sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::W:
                Audio::playSound("./res/walk.wav");
            case sf::Keyboard::A:
                Audio::playSound("./res/walk.wav");
            case sf::Keyboard::S:
                Audio::playSound("./res/walk.wav");
            case sf::Keyboard::D:
                Audio::playSound("./res/walk.wav");
        }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        Audio::playSound("./res/tick.wav");
    }
}