#ifndef _DUCK_HPP_
#define _DUCK_HPP_

#include <vector>
#include <cmath>
#include "entity.hpp"
#include "timer.hpp"

class Duck: public Entity {
private:

public:
    void initModel() override {
        model.push_back(
            Graphics::Quad(
                0.80,
                UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 7, 90.5 + 90.5 * 1), 
                UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 7, 0 + 90.5 * 1), 
                UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 7, 271.5 + 90.5 * 1),
                UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 7, 362 + 90.5 * 1)
            )
        );
        model.push_back(
            Graphics::Quad(
                0.90,
                UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 4, 90.5 + 90.5 * 2), 
                UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 4, 0 + 90.5 * 2), 
                UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 4, 271.5 + 90.5 * 2),
                UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 4, 362 + 90.5 * 2)
            )
        );
        model.push_back(
            Graphics::Quad(
                0.91,
                UIVec(-0.5, 0.5), sf::Vector2f(0 + 128, 90.5 + 90.5 * 3),
                UIVec(0.5, 0.5), sf::Vector2f(384 + 128, 0 + 90.5 * 3),
                UIVec(0.5, -0.5), sf::Vector2f(512 + 128, 271.5 + 90.5 * 3),
                UIVec(-0.5, -0.5), sf::Vector2f(128 + 128, 362 + 90.5 * 3)
            )
        );
        model[0].zPosScale = 0.; // shadow stays on the ground
    }

    Duck() {
        initModel();
    }

    void update() override {
        updateTimer();

        heading += headingRotationSpeed * elapsedSecs;
        position.x += velocity * std::cos(heading) * elapsedSecs;
        position.y += velocity * std::sin(heading) * elapsedSecs;
        position = position + slideVelocity * elapsedSecs;
        if (zPosition > 0)
            zVelocity += GRAVITY * elapsedSecs;
        zPosition += zVelocity;
        zPosition = std::max(zPosition, 0.);
        if (zPosition == 0.)
            zVelocity = 0.;
    }

};

#endif