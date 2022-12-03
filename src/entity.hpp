#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include "utilities.hpp"
#include "graphics.hpp"
#include "camera.hpp"
#include "timer.hpp"
#include "action.hpp"

#define OUT_OF_SIGHT 500
#define GRAVITY -1.

/// @brief Renderer and motion control for entities
class Entity {
public:
    Timer entityTimer;
    double lastUpdate = -1.;
    double elapsedSecs = 0.;

    coord position;
    double zPosition = 0.;
    double velocity = 0.;
    double zVelocity = 0.;
    coord slideVelocity; // TODO: get rid of this
    double heading = 0.;
    double headingRotationSpeed = 0.;
    
    std::vector<Graphics::Quad> model;

    std::vector<Action> actions;

    void runActions(std::vector<Action>& insertActionGlobal) {
        std::sort(actions.begin(), actions.end());
        std::vector<Action> insertActionEntity;
        for (int i = actions.size() - 1; i >= 0; i--) {
            if (actions[i].time.elapsed() >= 0) { // started
                runAction(actions[i], insertActionEntity, insertActionGlobal);
                actions[i].deleteFlag = true;
            }
        }
        for (auto& action : insertActionEntity)
            actions.push_back(action); // rerun next round
        std::sort(actions.begin(), actions.end());
        for (int i = actions.size() - 1; i >= 0; i--) {
            if (actions[i].deleteFlag)
                actions.resize(i);
        }
    }

    virtual void runAction(Action& action, std::vector<Action>& insertActionEntity, std::vector<Action>& insertActionGlobal) {

    }

    virtual void initModel() {}

    void pushQuads() {
        // UIVec pos = Camera::getScreenPos(position);
        // if (pos.x < -OUT_OF_SIGHT || pos.x > Camera::getViewport().size.x + OUT_OF_SIGHT
        //  || pos.y < -OUT_OF_SIGHT || pos.y > Camera::getViewport().size.y + OUT_OF_SIGHT) {
        //     return;
        // }
        // else {
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
        // }
    }

    void updateTimer() {
        double thisUpdate = entityTimer.elapsed();
        if (lastUpdate >= 0) {
            elapsedSecs = thisUpdate - lastUpdate;
        }
        lastUpdate = thisUpdate;
    }

    virtual void update() {
        updateTimer();


    }

};

#endif