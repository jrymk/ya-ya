#include "controls.h"
#include "game.h"

Controls::Controls(Game* game) :
        game(game) {
}

std::shared_ptr<Entity> Controls::getFacingEntity(std::shared_ptr<Entity> player, EntityType filter) {
    auto nearby = game->neighborsFinder.findNeighbors(player->position, 2., filter);
    std::shared_ptr<Entity> facingEntity = nullptr;
    double bestScore = 1e8;
    for (auto e: nearby) {
        if (e == player)
            continue;
        if (!e->facingHighlightable)
            continue;
        UIVec epos = Camera::getScreenPos(e->position);
        UIVec ppos = Camera::getScreenPos(player->position);
        double angle = std::abs(subtractAngle(player->position.angle(e->position), player->heading));
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

void Controls::handleKeyPress(enum sf::Keyboard::Key key) {
    if (key == sf::Keyboard::F1) {
        Graphics::showWireframe = !Graphics::showWireframe;
        debug << "Toggled wireframe: " << Graphics::showWireframe << "\n";
    }
    if (key == sf::Keyboard::F2) {
        for (int i = 0; i < debugGraphs.size(); i++)
            debugGraphs[i].resetRange();
    }
    if (key == sf::Keyboard::F3) {
        debugStream.str("");
    }
    if (key == sf::Keyboard::F4) {
        Graphics::debugOutOfSight = !Graphics::debugOutOfSight;
        debug << "Toggled debug out of sight: " << Graphics::debugOutOfSight << "\n";
    }
    if (key == sf::Keyboard::F6) {
        for (auto duck: game->entities) {
            if (duck.second->type == DUCK) {
                Action a(duck.second, Timer::getNow(), DUCK_DUCKWALK_TO_UNTIL);
                a.argCoord[0].x = Camera::getMouseCoord().x + (getRand() - .5);
                a.argCoord[0].y = Camera::getMouseCoord().y + (getRand() - .5);
                game->pushAction(a);
            }
        }
    }
    if (key == sf::Keyboard::F7) {
        for (auto &e: game->entities) {
            if (e.second->type == EntityType::DUCK) {
                Action a(Timer::getNow(), GLOBAL_DESTROY);
                a.argEntity[0] = e.second;
                game->pushAction(a);
//                game->destroyEntity(e.second->id);
            }
        }
    }
    if (key == sf::Keyboard::F11) {
        graphicsIsFullscreen = !graphicsIsFullscreen;
        Graphics::createWindow(graphicsIsFullscreen);
    }
    if (key == sf::Keyboard::Right) {
    }
    if (key == sf::Keyboard::Left) {
    }
}