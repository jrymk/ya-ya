#include "duck.h"
#include <iomanip>

void Duck::runAction(Action& action, std::vector<Action>& followUpActions) {
    switch (action.command) {
        case INIT: {
            followUpActions.push_back(Action(action.entity, Timer::getNow() + 5. + getRand() * 25., DUCK_LOOP_WANDER));
            followUpActions.push_back(Action(action.entity, Timer::getNow() + 10. + getRand() * 40., DUCK_LOOP_LAY_EGGS));
            followUpActions.push_back(Action(action.entity, Timer::getNow() + 5. + getRand() * 20., DUCK_LOOP_FIND_MATE));
            // followUpActions.push_back(Action(action.entity, Timer::getNow() + 50. + getRand() * 200., "death"));
            break;
        }
        case DUCK_LOOP_WANDER: {
            followUpActions.push_back(Action(action.entity, Timer::getNow() + 5. + getRand() * 25., DUCK_LOOP_WANDER));
            Action a(action.entity, Timer::getNow(), DUCK_DUCKWALK_TO_UNTIL);
            a.argCoord[0].x = position.x + getRand() * 3.;
            a.argCoord[0].y = position.y + getRand() * 3.;
            followUpActions.push_back(a);
            break;
        }
        case DUCK_LOOP_LAY_EGGS: {
            if (!genderIsMale) {
                {
                    Action a(Timer::getNow(), GLOBAL_LAY_EGG_FIND_NEARBY_MALE);
                    a.argEntity[0] = action.entity;
                    followUpActions.push_back(a);
                }
                followUpActions.push_back(Action(action.entity, Timer::getNow() + 10. + getRand() * 40., DUCK_LOOP_LAY_EGGS));
            }
            break;
        }
        case DUCK_LOOP_FIND_MATE: {
            if (genderIsMale) {
                {
                    Action a(Timer::getNow(), GLOBAL_FIND_MATE_FEMALE);
                    a.argEntity[0] = action.entity;
                    followUpActions.push_back(a);
                }
                followUpActions.push_back(Action(action.entity, Timer::getNow() + 5. + getRand() * 20., DUCK_LOOP_FIND_MATE));
            }
            break;
        }
        case DUCK_DEATH: {
            for (int i = 0; i < 10; i++) {
                Action a(action.entity, Timer::getNow() + .05 * i, ENTITY_SLIDE_INSTANT);
                a.argCoord[0] = coord(0.05, 0.);
                followUpActions.push_back(a);
                Action b(action.entity, Timer::getNow() + .05 * i + .0125, ENTITY_SLIDE_INSTANT);
                b.argCoord[0] = coord(0., 0.05);
                followUpActions.push_back(b);
                Action c(action.entity, Timer::getNow() + .05 * i + .025, ENTITY_SLIDE_INSTANT);
                c.argCoord[0] = coord(-0.05, 0.);
                followUpActions.push_back(c);
                Action d(action.entity, Timer::getNow() + .05 * i + .0375, ENTITY_SLIDE_INSTANT);
                d.argCoord[0] = coord(0., -0.05);
                followUpActions.push_back(d);
            }
            {
                Action a(Timer::getNow() + .5, GLOBAL_DESTROY);
                a.argEntity[0] = action.entity;
                followUpActions.push_back(a);
            }
            break;
        }
        case DUCK_HAVE_SEX_WITH: {
            for (int i = 0; i < 20; i++) {
                Action a(action.entity, Timer::getNow() + .1 * i, ENTITY_SLIDE_INSTANT);
                a.argCoord[0] = coord(0.05, 0.);
                followUpActions.push_back(a);
                Action b(action.entity, Timer::getNow() + .1 * i + .025, ENTITY_SLIDE_INSTANT);
                b.argCoord[0] = coord(0., 0.05);
                followUpActions.push_back(b);
                Action c(action.entity, Timer::getNow() + .1 * i + .05, ENTITY_SLIDE_INSTANT);
                c.argCoord[0] = coord(-0.05, 0.);
                followUpActions.push_back(c);
                Action d(action.entity, Timer::getNow() + .1 * i + .075, ENTITY_SLIDE_INSTANT);
                d.argCoord[0] = coord(0., -0.05);
                followUpActions.push_back(d);
            }
            if (!genderIsMale) {
                if (getRand() < .9) { // 90%
                    {
                        Action a(Timer::getNow() + 3. + getRand() * 2., GLOBAL_LAY_FERTILIZED_EGG);
                        a.argCoord[0] = position;
                        a.argBool[0] = (getRand() > .5);
                        followUpActions.push_back(a);
                    }
                    if (getRand() < .1) {// 10% a twin!
                        Action a(Timer::getNow() + 3. + getRand() * 2., GLOBAL_LAY_FERTILIZED_EGG);
                        a.argCoord[0].x = position.x + 0.3 *(getRand() - .5);
                        a.argCoord[0].y = position.y + 0.3 *(getRand() - .5);
                        a.argBool[0] = (getRand() > .5);
                        followUpActions.push_back(a);
                    }
                }
                else {
                    Action a(Timer::getNow() + 3. + getRand() * 2., GLOBAL_LAY_UNFERTILIZED_EGG);
                    a.argCoord[0] = position;
                    followUpActions.push_back(a);
                }
            }
            break;
        }
        case DUCK_DUCKWALK_TO_UNTIL: {
            if (position.len(action.argCoord[0]) > 2.) {
                coord randTarget = action.argCoord[0] + coord((getRand() * 2. - 1.), (getRand() * 2. - 1.));
                if (std::abs(subtractAngle(heading, randTarget.angle(position))) > 1)
                    heading = position.angle(randTarget) + .2 * (getRand() * .4 - .2);
                headingRotationSpeed = -0.2 * subtractAngle(heading, position.angle(randTarget)) + (getRand() * .1 - .05);
                velocity = 4. + (getRand() * 2. - 1.);
            }
            else {
                heading = position.angle(action.argCoord[0]);
                velocity = 3. + (getRand() * 2. - 1.);
            }

            if (position.len(action.argCoord[0]) < .7) {
                velocity = 0.;
                headingRotationSpeed = 0.;
            }
            else {
                Action a(action.entity, Timer::getNow() + 0.2 * std::sqrt(position.len(action.argCoord[0])), DUCK_DUCKWALK_TO_UNTIL);
                a.argCoord[0] = action.argCoord[0];
                followUpActions.push_back(a);
            }
            break;
        }
        case DUCK_LAY_UNFERTILIZED_EGG: {
            Action a(Timer::getNow(), GLOBAL_LAY_UNFERTILIZED_EGG);
            a.argCoord[0] = position;
            followUpActions.push_back(a);
            break;
        }
        case DUCK_LAY_FERTILIZED_EGG: {
            break;
        }
        case DUCK_RESULT_FIND_MATE_FEMALE: {
            Action a(action.entity, Timer::getNow() + .1, DUCK_DUCKWALK_TO_UNTIL);
            a.argCoord[0] = action.argCoord[0];
            followUpActions.push_back(a);
            break;
        }
    }
}

void Duck::initModel() {
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

Duck::Duck() {
    // auto shared = std::make_shared<Duck>(this);
    // Entity::childClassPtr = std::dynamic_pointer_cast<Entity>(shared);
    type = DUCK;
    initModel();
}

void Duck::customUpdate() {
    if (!motionFrozen) {
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
}

std::string Duck::getDescriptionStr() {
    std::stringstream ss;
    ss << "id: " << id << "\n";
    ss << "type: " << type << "\n";
    ss << "position: " << std::setprecision(3) << std::fixed << position.x << ", " << position.y << "\n";
    ss << "chunk: " << neighborsFinderMyTile.first << ", " << neighborsFinderMyTile.second << "\n";
    ss << "gender: " << (genderIsMale ? "male" : "female") << "\n";
    return ss.str();
}