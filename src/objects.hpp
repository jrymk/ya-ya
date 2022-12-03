#ifndef _OBJECTS_HPP_
#define _OBJECTS_HPP_

#include "entity.hpp"

class Egg : public Entity {
public:
    // void runActions() {
    //     std::sort(actions.begin(), actions.end());
    //     std::vector<EggAction> insertActions;
    //     for (int i = actions.size() - 1; i >= 0; i--) {
    //         if (actions[i].actionBeginTime.elapsed() >= 0) { // started
    //             runAction(actions[i], insertActions);
    //             if (actions[i].rerun) 
    //                 insertActions.push_back(actions[i]);
    //             actions[i].deleteFlag = true;
    //         }
    //     }

    //     for (auto& action : insertActions)
    //         insertAction(action.actionBeginTime, action.action);
    //     std::sort(actions.begin(), actions.end());
    //     for (int i = actions.size() - 1; i >= 0; i--) {
    //         if (actions[i].deleteFlag)
    //             actions.resize(i);
    //     }
    // }

    void runAction(Action& action, std::vector<Action>& insertActionEntity, std::vector<Action>& insertActionGlobal) override {
        std::stringstream ss(action.action); 
        std::string function;
        while (ss >> function) {
            if (function == "hatch") {
                insertActionGlobal.push_back(Entity::Action(Timer::getNow(), "hatch " + toString(position.x) + " " + toString(position.y)));
                
                // delete this;

            }
            if (function == "hop") {
                if (zPosition == 0.)
                    zVelocity = .2;
            }
        }
    }

    void initModel() override {
        model.push_back(
            Graphics::Quad(
                0.85,
                UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 10, 90.5 + 90.5 * 0), 
                UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 10, 0 + 90.5 * 0), 
                UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 10, 271.5 + 90.5 * 0),
                UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 10, 362 + 90.5 * 0)
            )
        );
    }

    Egg() {
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