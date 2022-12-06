#ifndef _CONTROLS_HPP_
#define _CONTROLS_HPP_

#include <map>
#include "entity.hpp"
#include "camera.hpp"
#include "neighborsfinder.hpp"

class Controls {
    std::map<std::string, Entity*>* entities;
    NeighborsFinder* neighborsFinder;

public:
    Controls(std::map<std::string, Entity*>* entities, NeighborsFinder* neighborsFinder): 
        entities(entities),
        neighborsFinder(neighborsFinder) {
    }

    Entity* getFacingEntity(Entity* player, const std::string& filter = "") {
        auto nearby = neighborsFinder->findNeighbors(player->position, 2., filter);
        Entity* facingEntity = nullptr;
        double bestScore = 1e8;
        for (auto e : nearby) {
            if (e == player)
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

    void handleKeyPress(enum sf::Keyboard::Key key) {
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
            // game.showActionList = !game.showActionList;
            // debug << "Toggled action list: " << game.showActionList << "\n";
        }
        if (key == sf::Keyboard::F11) {
            graphicsIsFullscreen = !graphicsIsFullscreen;
            Graphics::createWindow(graphicsIsFullscreen);
        }
        if (key == sf::Keyboard::Right) {
        }
        if (key == sf::Keyboard::Left) {
        }
        if (key == sf::Keyboard::P) {
            updatePhysics = true;
        }
    }
};

#endif