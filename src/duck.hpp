#ifndef _DUCK_HPP_
#define _DUCK_HPP_

#include <vector>
#include <sstream>
#include <cmath>
#include <iomanip>
#include "entity.h"
#include "timer.h"
#include "serialization.h"

class Duck : public Entity {
private:

public:
    void runAction(Action& action, std::vector<Action>& followUpActions) override {
        std::stringstream ss(action.action); 
        std::string function;
        while (ss >> function) {
            if (function == "init") {
                followUpActions.push_back(Action(id, Timer::getNow() + 5. + getRand() * 25., "loop_wander"));
                followUpActions.push_back(Action(id, Timer::getNow() + 10. + getRand() * 40., "loop_lay_eggs"));
                followUpActions.push_back(Action(id, Timer::getNow() + 5. + getRand() * 20., "loop_find_mate"));
                // followUpActions.push_back(Action(id, Timer::getNow() + 50. + getRand() * 200., "death"));
            }
            if (function == "loop_wander") {
                followUpActions.push_back(Action(id, Timer::getNow() + 5. + getRand() * 25., "loop_wander"));
                followUpActions.push_back(Action(id, Timer::getNow(), "duckwalk_to_until " + toStr(position.x + getRand() * 3.) + " " + toStr(position.y + getRand() * 3.)));
            }
            if (function == "loop_lay_eggs") {
                if (!genderIsMale) {
                    followUpActions.push_back(Action("global", Timer::getNow(), "lay_egg_find_nearby_male " + id));
                    followUpActions.push_back(Action(id, Timer::getNow() + 10. + getRand() * 40., "loop_lay_eggs"));
                }
            }
            if (function == "loop_find_mate") {
                if (genderIsMale) {
                    followUpActions.push_back(Action(id, Timer::getNow() + 5. + getRand() * 20., "loop_find_mate"));
                    followUpActions.push_back(Action("global", Timer::getNow(), "find_mate_female " + id));
                }
            }
            if (function == "death") {
                for (int i = 0; i < 10; i++) {
                    followUpActions.push_back(Action(id, Timer::getNow() + .05 * i, "slide_instant 0.05 0"));    
                    followUpActions.push_back(Action(id, Timer::getNow() + .05 * i + .0125, "slide_instant 0 0.05"));    
                    followUpActions.push_back(Action(id, Timer::getNow() + .05 * i + .025, "slide_instant -0.05 0"));    
                    followUpActions.push_back(Action(id, Timer::getNow() + .05 * i + .0375, "slide_instant 0 -0.05"));    
                }
                followUpActions.push_back(Action("global", Timer::getNow() + .5, "destroy " + id));
            }
            if (function == "have_sex_with") {
                for (int i = 0; i < 20; i++) {
                    followUpActions.push_back(Action(id, Timer::getNow() + .1 * i, "slide_instant 0.05 0"));    
                    followUpActions.push_back(Action(id, Timer::getNow() + .1 * i + .025, "slide_instant 0 0.05"));    
                    followUpActions.push_back(Action(id, Timer::getNow() + .1 * i + .05, "slide_instant -0.05 0"));    
                    followUpActions.push_back(Action(id, Timer::getNow() + .1 * i + .075, "slide_instant 0 -0.05"));    
                }
                if (!genderIsMale) {
                    if (getRand() < .9) { // 90% 
                        followUpActions.push_back(Action("global", Timer::getNow() + 3. + getRand() * 2., "lay_fertilized_egg " + toStr(position.x) + " " + toStr(position.y) + " " + toStr(getRand() > .5)));
                        if (getRand() < .1) // 10% a twin!
                            followUpActions.push_back(Action("global", Timer::getNow() + 3. + getRand() * 2., "lay_fertilized_egg " + toStr(position.x + 0.3 *(getRand() - .5)) + " " + toStr(position.y + 0.3 *(getRand() - .5)) + " " + toStr(getRand() > .5)));
                    }
                    else 
                        followUpActions.push_back(Action("global", Timer::getNow() + 3. + getRand() * 2., "lay_unfertilized_egg " + toStr(position.x) + " " + toStr(position.y)));
                }
            }
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

                if (position.len(target) < .7) {
                    velocity = 0.;
                    headingRotationSpeed = 0.;
                }
                else {
                    followUpActions.push_back(Action(id, Timer::getNow() + 0.2 * std::sqrt(position.len(target)), "duckwalk_to_until " + toStr(target.x) + " " + toStr(target.y)));
                }
            }
            if (function == "lay_unfertilized_egg") {
                followUpActions.push_back(Action("global", Timer::getNow(), "lay_unfertilized_egg " + toStr(position.x) + " " + toStr(position.y)));
            }
            if (function == "lay_fertilized_egg") {
            }
            if (function == "slide_instant") {
                coord delta;
                ss >> delta.x >> delta.y;
                position = position + delta;
            }
            if (function == "slide_velocity") {
                coord velocity;
                ss >> velocity.x >> velocity.y;
                position = position + velocity / elapsedSecs;
            }
            if (function == "slide_velocity_distance") {
                coord velocity;
                double distance;
                ss >> velocity.x >> velocity.y >> distance;
                position = position + velocity / velocity.len() * std::min(velocity.len() * elapsedSecs, distance);
                distance -= velocity.len() * elapsedSecs;
                velocity = velocity / velocity.len() * std::max(distance / 0.5, velocity.len() - 0.05 * elapsedSecs); // deccelerate
                if (distance > 0.)
                    followUpActions.push_back(Action(id, Timer::getNow(), "slide_velocity_distance " + toStr(velocity.x) + " " + toStr(velocity.y) + " " + toStr(distance)));
            }
            if (function == "result_find_mate_female") {
                std::string mateid;
                coord pos;
                ss >> mateid >> pos.x >> pos.y;
                followUpActions.push_back(Action(id, Timer::getNow() + .1, "duckwalk_to_until " + toStr(pos.x) + " " + toStr(pos.y)));
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
        type = "duck";
        initModel();
    }

    void customUpdate() override {
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

    std::string getDescriptionStr() override {
        std::stringstream ss;
        ss << "id: " << id << "\n";
        ss << "type: " << type << "\n";
        ss << "position: " << std::setprecision(3) << std::fixed << position.x << ", " << position.y << "\n";
        ss << "chunk: " << neighborsFinderMyTile.first << ", " << neighborsFinderMyTile.second << "\n";
        ss << "gender: " << (genderIsMale ? "male" : "female") << "\n";
        return ss.str();
    }

    constexpr static auto properties = std::make_tuple(
        SaveUtilities::property(&Duck::position, "Dk.ps")
    );
};

#endif