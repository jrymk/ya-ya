#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include "utilities.hpp"
#include "graphics.hpp"
#include "camera.hpp"
#include "timer.hpp"

#define OUT_OF_SIGHT 500

/// @brief Renderer and motion control for entities
class Entity {
public:
    Timer entityTimer;
    double lastUpdate = -1.;
    double elapsedSecs = 0.;
    
    coord position;
    double velocity = 0.;
    coord slideVelocity;
    double heading = 0.;
    double headingRotationSpeed = 0.;

    std::vector<Graphics::Quad> model;

    virtual void initModel() {}

    void pushQuads() {
        UIVec pos = Camera::getScreenPos(position);
        if (pos.x < -OUT_OF_SIGHT || pos.x > Camera::getViewport().size.x + OUT_OF_SIGHT
         || pos.y < -OUT_OF_SIGHT || pos.y > Camera::getViewport().size.y + OUT_OF_SIGHT) {
            return;
        }
        else {
            for (auto quad : model) {
                quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) + position);
                quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) + position);
                quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) + position);
                quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) + position);
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
                        Camera::getScreenPos(position) + UIVec(5., 0.),
                        Camera::getScreenPos(position) + UIVec(0., 5.),
                        Camera::getScreenPos(position) + UIVec(-5., 0.),
                        Camera::getScreenPos(position) + UIVec(0., -5.),
                        sf::Color::Magenta, sf::Color::Black
                    );
                }
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

    virtual void update() {
        updateTimer();


    }

};

#endif