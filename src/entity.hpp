#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include "utilities.hpp"
#include "graphics.hpp"
#include "camera.hpp"
#include "timer.hpp"

#define OUT_OF_SIGHT 500

/// @brief Renderer and motion control for entities
class Entity: public Model {
public:
    coord force;
    double mass = .1;
    double staticFrictionMaxForce = 14.;
    double kineticFrictionForce = 12.;
    coord acceleration;
    coord velocity;
    coord position;
    double maxVelocity = 25.;
    Timer physicsTimer;

    void pushModel() override {
        UIVec pos = Camera::getScreenPos(position);
        if (pos.x < -OUT_OF_SIGHT || pos.x > Camera::getViewport().size.x + OUT_OF_SIGHT
         || pos.y < -OUT_OF_SIGHT || pos.y > Camera::getViewport().size.y + OUT_OF_SIGHT) {
            return;
        }
        else {
            for (auto quad : Model::quads) {
                quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) + position);
                quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) + position);
                quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) + position);
                quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) + position);
                Graphics::insertQuad(quad);
            }
        }
    }

    void updatePhysics() {
        physicsTimer.tick();

        acceleration = force / mass;
        if (velocity.len() > 1e-1) { // moving -> kinetic friction
            if (acceleration.len() < kineticFrictionForce / mass) 
                acceleration = acceleration - (velocity / velocity.len() * kineticFrictionForce / mass);
            else 
                acceleration = acceleration - (acceleration / acceleration.len() * kineticFrictionForce / mass);
        }
        else { // not moving -> clear acceration or subtract acceleration
            if (acceleration.len() < staticFrictionMaxForce / mass)
                acceleration = coord(0, 0);
            else 
                acceleration = acceleration - (acceleration / acceleration.len() * staticFrictionMaxForce / mass);
        }
        velocity = velocity + (acceleration * ((double)physicsTimer.getTickElapsed() / 1e6));
        // due to time accuracies, we assume static friction kicks in when velocity switch sides
        if (velocity.x * (velocity - (acceleration * ((double)physicsTimer.getTickElapsed() / 1e6))).x < 0)
            velocity.x = 0;
        if (velocity.y * (velocity - (acceleration * ((double)physicsTimer.getTickElapsed() / 1e6))).y < 0)
            velocity.y = 0;

        if (velocity.len() < 1e-1)
            velocity = coord(0, 0);
        if (velocity.len() > maxVelocity)
            velocity = velocity * (maxVelocity / velocity.len());
        
        position = position + velocity * ((double)physicsTimer.getTickElapsed() / 1e6);
        force = coord(0, 0);
    }

};

#endif