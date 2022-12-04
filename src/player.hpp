#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <vector>
#include <cmath>
#include "entity.hpp"
#include "timer.hpp"

class Player: public Entity {
private:

public:
    void initModel() override {
        model.push_back(
            Graphics::Quad(
                0.80,
                UIVec(0.5, -0.5), sf::Vector2f(0 + 128 * 7, 90.5 + 90.5 * 1), 
                UIVec(-0.5, -0.5), sf::Vector2f(384 + 128 * 7, 0 + 90.5 * 1), 
                UIVec(-0.5, 0.5), sf::Vector2f(512 + 128 * 7, 271.5 + 90.5 * 1),
                UIVec(0.5, 0.5), sf::Vector2f(128 + 128 * 7, 362 + 90.5 * 1)
            )
        );
        model.push_back(
            Graphics::Quad(
                0.91,
                UIVec(-0.5, 1.5), sf::Vector2f(0 + 128 * 2, 90.5 + 90.5 * 6),
                UIVec(0.5, 1.5), sf::Vector2f(384 + 128 * 2, 0 + 90.5 * 6),
                UIVec(0.5, 0.5), sf::Vector2f(512 + 128 * 2, 271.5 + 90.5 * 6),
                UIVec(-0.5, 0.5), sf::Vector2f(128 + 128 * 2, 362 + 90.5 * 6)
            )
        );
        model.push_back(
            Graphics::Quad(
                0.91,
                UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 3, 90.5 + 90.5 * 9),
                UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 3, 0 + 90.5 * 9),
                UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 3, 271.5 + 90.5 * 9),
                UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 3, 362 + 90.5 * 9)
            )
        );
    }

    Player() {
        initModel();
    }

    void customUpdate() override {
        heading += headingRotationSpeed * elapsedSecs;
        position.x += velocity * std::cos(heading) * elapsedSecs;
        position.y += velocity * std::sin(heading) * elapsedSecs;
        position = position + slideVelocity * elapsedSecs;
    }

};

#endif