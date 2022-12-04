#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include "utilities.hpp"
#include "graphics.hpp"
#include "camera.hpp"
#include "timer.hpp"
#include "action.hpp"
#include <deque>

#define OUT_OF_SIGHT 500
#define GRAVITY -1.

/// @brief Renderer and motion control for entities
class Entity {
public:
    std::string id = "undefined";
    std::string type = "entity";

    Timer entityTimer;
    double lastUpdate = -1.;
    double elapsedSecs = 0.;

    coord position;
    std::deque<std::pair<Timer, coord>> historyPosition;
    double zPosition = 0.;
    double velocity = 0.;
    double zVelocity = 0.;
    coord slideVelocity; // TODO: get rid of this
    double heading = 0.;
    double headingRotationSpeed = 0.;
    
    std::vector<Graphics::Quad> model;

    virtual void runAction(Action& action, std::vector<Action>& followUpActions) {
        
    }

    virtual void initModel() {}

    void pushQuads() {
        for (auto quad : model) {
            quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) + position) + UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
            quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) + position) + UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
            quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) + position) + UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
            quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) + position) + UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
            quad.zDepth += (Camera::getScreenPos(position).y / Camera::getViewport().size.y - 0.5) / 100.;
            Graphics::insertQuad(quad);

            if (Graphics::showWireframe) {
                Graphics::insertUserWireframe(
                    Camera::getScreenPos(position),
                    Camera::getScreenPos(position),
                    Camera::getScreenPos(position) + Camera::getAngleVector(0.3, heading),
                    Camera::getScreenPos(position) + Camera::getAngleVector(0.3, heading),
                    sf::Color::Yellow, sf::Color::Black
                );
                Graphics::insertUserWireframe(
                    Camera::getScreenPos(position) + UIVec(5., 0.) + UIVec(0, -zPosition * Camera::getScale()),
                    Camera::getScreenPos(position) + UIVec(0., 5.) + UIVec(0, -zPosition * Camera::getScale()),
                    Camera::getScreenPos(position) + UIVec(-5., 0.) + UIVec(0, -zPosition * Camera::getScale()),
                    Camera::getScreenPos(position) + UIVec(0., -5.) + UIVec(0, -zPosition * Camera::getScale()),
                    sf::Color::Magenta, sf::Color::Black
                );
            }
        }
    }

    void updateTimer() {
        double thisUpdate = entityTimer.elapsed();
        if (lastUpdate >= 0) {
            elapsedSecs = thisUpdate - lastUpdate;
        }
        lastUpdate = thisUpdate;
    }

    void update() {
        updateTimer();
        historyPosition.push_front({Timer::getNow(), position});
        while (!historyPosition.empty() && historyPosition.front().first.elapsed() >= 1.0)
            historyPosition.pop_back();
        
        customUpdate();
    }

    virtual void customUpdate() {

    }
};

#endif