#include "controls.h"
#include "game.h"
#include "player.h"

Controls::Controls(Game* game) :
        game(game) {
}

void Controls::update() {
    facingEntity = getFacingEntity();
    facingTile = &getFacingTile();
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
                for (int i = 0; i < debugGraphs.size(); i++)
                    debugGraphs[i].resetRange();
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)) {
            if (facingTile != nullptr) {
                facingTile->setTileType(Map::Tile::STONE);
            }
        } else {
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                if (game->player->inventory[Player::InventorySlots::LEFT_HAND] == nullptr && facingEntity) {
                    Action a(facingEntity, Timer::getNow(), ENTITY_OWN_BY);
                    a.argEntity[0] = game->player;
                    a.argInt[0] = Player::InventorySlots::LEFT_HAND;
                    game->pushAction(a);
                } else if (game->player->inventory[Player::InventorySlots::LEFT_HAND] != nullptr) {
                    game->pushAction(Action(game->player->inventory[Player::InventorySlots::LEFT_HAND], Timer::getNow(), ENTITY_UNOWN));
                }
            }
            if (event.mouseButton.button == sf::Mouse::Button::Right) {
                if (game->player->inventory[Player::InventorySlots::RIGHT_HAND] == nullptr && facingEntity) {
                    Action a(facingEntity, Timer::getNow(), ENTITY_OWN_BY);
                    a.argEntity[0] = game->player;
                    a.argInt[0] = Player::InventorySlots::RIGHT_HAND;
                    game->pushAction(a);
                } else if (game->player->inventory[Player::InventorySlots::RIGHT_HAND] != nullptr) {
                    game->pushAction(Action(game->player->inventory[Player::InventorySlots::RIGHT_HAND], Timer::getNow(), ENTITY_UNOWN));
                }
            }
        }
    }
}

