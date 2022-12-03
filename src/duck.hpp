#ifndef _DUCK_HPP_
#define _DUCK_HPP_

#include <vector>
#include <sstream>
#include <cmath>
#include "entity.hpp"
#include "timer.hpp"

class Duck : public Entity {
private:

public:
    void runAction(Action& action, std::vector<Action>& insertActionEntity, std::vector<Action>& insertActionGlobal) override {
        std::stringstream ss(action.action); 
        std::string function;
        while (ss >> function) {
            if (function == "rerun")
                action.rerun = true;
            if (function == "hop") {
                if (zPosition == 0.)
                    zVelocity = .2;
            }
            if (function == "duckwalk_to_until") {
                coord target;
                ss >> target.x >> target.y;

                if (position.len(target) > 2.) {
                    coord randTarget = target + coord((getRand() * 2. - 1.), (getRand() * 2. - 1.));
                    if (std::abs(subtractAngle(heading, randTarget.angle(position))) > 1)
                        heading = position.angle(randTarget) + .2 * (getRand() * .4 - .2);
                    headingRotationSpeed = -0.2 * subtractAngle(heading, position.angle(randTarget)) + (getRand() * .1 - .05);
                    velocity = 4. + (getRand() * 2. - 1.);
                    
                }
                else {
                    heading = position.angle(target);
                    velocity = 3. + (getRand() * 2. - 1.);
                }

                if (position.len(target) < .1) {
                    velocity = 0.;
                    headingRotationSpeed = 0.;
                }
                else {
                    insertActionEntity.push_back(Action(Timer::getNow() + 0.2 * std::sqrt(position.len(target)), "duckwalk_to_until " + toString(target.x) + " " + toString(target.y)));
                }
            }
            if (function == "lay_egg") {
                insertActionGlobal.push_back(Action(Timer::getNow(), "lay_egg " + toString(position.x) + " " + toString(position.y)));
            }
        }
    }

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