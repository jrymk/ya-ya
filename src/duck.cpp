#include "duck.h"
#include <iomanip>
#include "game.h"
#include "egg.h"

void Duck::runAction(Action &action, std::vector<Action> &followUpActions) {
    switch (action.command) {
        case ON_CREATION:
            followUpActions.emplace_back(action.entity, Timer::getNow() + 5. + getRand() * 25., DUCK_LOOP_WANDER);
            if (genderIsMale) {
                followUpActions.emplace_back(action.entity, Timer::getNow() + 5. + getRand() * 20., DUCK_LOOP_FIND_MATE);
            } else {
                followUpActions.emplace_back(action.entity, Timer::getNow() + 10. + getRand() * 40., DUCK_LOOP_LAY_EGGS);
            }
            followUpActions.emplace_back(action.entity, Timer::getNow(), ENTITY_HOP);
            followUpActions.emplace_back(action.entity, Timer::getNow() + 50. + getRand() * 200., DUCK_DEATH);

//            position = action.argEntity[0]->position + coord::getRandCoord();
            {
                Action a(action.entity, Timer::getNow() + .3, DUCK_DUCKWALK_TO_UNTIL);
                a.argCoord[0].x = position.x + 3. * (getRand() - 0.5);
                a.argCoord[0].y = position.y + 3. * (getRand() - 0.5);
                followUpActions.push_back(a);
            }
            selectable = true;
            break;
        case ON_UNOWNED:

            break;
        case DUCK_LOOP_WANDER: {
            followUpActions.emplace_back(action.entity, Timer::getNow() + 5. + getRand() * 25., DUCK_LOOP_WANDER);
            Action a(action.entity, Timer::getNow(), DUCK_DUCKWALK_TO_UNTIL);
            a.argCoord[0].x = position.x + getRand() * 3.;
            a.argCoord[0].y = position.y + getRand() * 3.;
            followUpActions.push_back(a);
            break;
        }
        case DUCK_LOOP_LAY_EGGS: { // female only
            if (fertilized) {
                {
                    std::shared_ptr<Egg> egg(new Egg(game));
                    egg->id = game->newId(EGG);
                    egg->position = position + coord::getRandCoord();
                    egg->genderIsMale = (getRand() > .5);
                    egg->fertilized = true;
                    auto &egg_ptr = game->insertEntity(egg);
                    {
                        Action a(egg_ptr, Timer::getNow(), ENTITY_OWN_BY);
                        a.argEntity[0] = action.entity;
                        a.argInt[0] = InventorySlots::EGG_0;
                        followUpActions.push_back(a);
                    }
                    {
                        Action a(egg_ptr, Timer::getNow() + 3. + getRand() * 2., ENTITY_UNOWN);
                        followUpActions.push_back(a);
                    }
                }

                if (getRand() < .1) {// 10% a twin!
                    std::shared_ptr<Egg> egg(new Egg(game));
                    egg->id = game->newId(EGG);
                    egg->position = position + coord::getRandCoord();
                    egg->genderIsMale = (getRand() > .5);
                    egg->fertilized = true;
                    auto &egg_ptr = game->insertEntity(egg);
                    {
                        Action a(egg_ptr, Timer::getNow(), ENTITY_OWN_BY);
                        a.argEntity[0] = action.entity;
                        a.argInt[0] = InventorySlots::EGG_1;
                        followUpActions.push_back(a);
                    }
                    {
                        Action a(egg_ptr, Timer::getNow() + 3. + getRand() * 2., ENTITY_UNOWN);
                        followUpActions.push_back(a);
                    }
                }
                fertilized = false;
            } else {
                std::shared_ptr<Egg> egg(new Egg(game));
                egg->id = game->newId(EGG);
                egg->position = position + coord::getRandCoord();
                egg->fertilized = false;
                auto &egg_ptr = game->insertEntity(egg);
                {
                    Action a(egg_ptr, Timer::getNow(), ENTITY_OWN_BY);
                    a.argEntity[0] = action.entity;
                    a.argInt[0] = InventorySlots::EGG_0;
                    followUpActions.push_back(a);
                }
                {
                    Action a(egg_ptr, Timer::getNow() + 3. + getRand() * 2., ENTITY_UNOWN);
                    followUpActions.push_back(a);
                }
            }
            followUpActions.emplace_back(action.entity, Timer::getNow() + 10. + getRand() * 40., DUCK_LOOP_LAY_EGGS);
            break;
        }
        case DUCK_LOOP_FIND_MATE: { // male only
            {
                auto result = game->neighborsFinder.findNeighbors(position, 8.);
                std::shared_ptr<Entity> closest = nullptr;
                std::vector<std::shared_ptr<Entity>> candidates;
                for (auto f: result) {
                    if (f->type != DUCK)
                        continue;
                    auto duck = std::dynamic_pointer_cast<Duck>(f);
                    if (!duck)
                        continue;
                    if (!duck->genderIsMale) {
                        if (!closest || f->position.len(position) < closest->position.len(position))
                            closest = f;
                        candidates.push_back(f);
                    }
                }
                if (closest) {
                    if (getRand() < .4) // change mate?
                        closest = candidates[std::min(int(getRand() * candidates.size()), int(candidates.size() - 1))];

                    {
                        Action a(action.entity, Timer::getNow() + .1, DUCK_DUCKWALK_TO_UNTIL);
                        a.argCoord[0] = closest->position;
                        followUpActions.push_back(a);
                    }
                    {
                        Action a(Timer::getNow() + .1, DUCK_UNTIL_MATE_CONTACT);
                        a.argEntity[0] = closest;
                        followUpActions.push_back(a);
                    }
                }
            }
            followUpActions.emplace_back(action.entity, Timer::getNow() + 5. + getRand() * 20., DUCK_LOOP_FIND_MATE);
            break;
        }
        case DUCK_UNTIL_MATE_CONTACT: {
            if (position.len(action.argEntity[0]->position) < .2) { // have contact
                {
                    Action a(action.argEntity[0], Timer::getNow() + .3, DUCK_HAVE_SEX_WITH);
                    a.argEntity[0] = action.entity; // me
                    followUpActions.push_back(a);
                }
                {
                    Action a(action.entity, Timer::getNow() + .3, DUCK_HAVE_SEX_WITH);
                    a.argEntity[0] = action.argEntity[0];
                    followUpActions.push_back(a);
                }
            } else {
                Action a(Timer::getNow() + .1, DUCK_UNTIL_MATE_CONTACT);
                a.argEntity[0] = action.argEntity[0];
                followUpActions.push_back(a);
            }
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
            {
                Action a(action.entity, Timer::getNow(), ENTITY_MOTION_FROZEN);
                a.argBool[0] = true;
                followUpActions.push_back(a);
            }
//            for (int i = 0; i < 20; i++) {
//                Action a(action.entity, Timer::getNow() + .1 * i, ENTITY_SLIDE_INSTANT);
//                a.argCoord[0] = coord(0.05, 0.);
//                followUpActions.push_back(a);
//                Action b(action.entity, Timer::getNow() + .1 * i + .025, ENTITY_SLIDE_INSTANT);
//                b.argCoord[0] = coord(0., 0.05);
//                followUpActions.push_back(b);
//                Action c(action.entity, Timer::getNow() + .1 * i + .05, ENTITY_SLIDE_INSTANT);
//                c.argCoord[0] = coord(-0.05, 0.);
//                followUpActions.push_back(c);
//                Action d(action.entity, Timer::getNow() + .1 * i + .075, ENTITY_SLIDE_INSTANT);
//                d.argCoord[0] = coord(0., -0.05);
//                followUpActions.push_back(d);
//            }
            if (genderIsMale) {
                heading = position.angle(action.argEntity[0]->position);
                for (int i = 0; i < 20; i++) {
                    Action a(action.entity, Timer::getNow() + .1 * i, ENTITY_SLIDE_INSTANT);
                    a.argCoord[0] = coord::getAngleVec(.1, heading);
                    followUpActions.push_back(a);
                    Action c(action.entity, Timer::getNow() + .1 * i + .05, ENTITY_SLIDE_INSTANT);
                    c.argCoord[0] = coord::getAngleVec(-.1, heading);
                    followUpActions.push_back(c);
                }
            } else {
                heading = action.argEntity[0]->position.angle(position);
                for (int i = 0; i < 20; i++) {
                    Action a(action.entity, Timer::getNow() + .1 * i, ENTITY_SLIDE_INSTANT);
                    a.argCoord[0] = coord::getAngleVec(.02, heading);
                    followUpActions.push_back(a);
                    Action c(action.entity, Timer::getNow() + .1 * i + .05, ENTITY_SLIDE_INSTANT);
                    c.argCoord[0] = coord::getAngleVec(-.02, heading);
                    followUpActions.push_back(c);
                }
            }

            {
                Action a(action.entity, Timer::getNow() + 2., ENTITY_MOTION_FROZEN);
                a.argBool[0] = false;
                followUpActions.push_back(a);
            }
            {
                Action a(action.entity, Timer::getNow() + 2., DUCK_FINISH_SEX);
                a.argEntity[0] = action.argEntity[0];
                followUpActions.push_back(a);
            }
            break;
        }
        case DUCK_FINISH_SEX: {
            if (!genderIsMale) {
                if (getRand() < .9) // 90% success sex
                    fertilized = true;
            } else {
                // happy
            }
            break;
        }
        case DUCK_DUCKWALK_TO_UNTIL: {
            if (position.len(action.argCoord[0]) > 2.) {
                coord randTarget = action.argCoord[0] + coord((getRand() * .3 - .15), (getRand() * .3 - .15));
                if (std::abs(subtractAngle(heading, position.angle(randTarget))) > 1.)
                    heading = position.angle(randTarget) + .2 * (getRand() * .4 - .2);
                headingRotationSpeed = -0.2 * subtractAngle(heading, position.angle(randTarget)) + (getRand() * .1 - .05);
                velocity = 4. + (getRand() * 2. - 1.);
            } else {
                heading = position.angle(action.argCoord[0]);
                velocity = 3. + (getRand() * 2. - 1.);
            }

            if (position.len(action.argCoord[0]) < .7) {
                velocity = 0.;
                headingRotationSpeed = 0.;
            } else {
                Action a(action.entity, Timer::getNow() + 0.2 * std::sqrt(position.len(action.argCoord[0])), DUCK_DUCKWALK_TO_UNTIL);
                a.argCoord[0] = action.argCoord[0];
                followUpActions.push_back(a);
            }
            break;
        }
    }
}

void Duck::initModel() {
    model.emplace_back(
            0.80,
            UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 7, 90.5 + 90.5 * 1),
            UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 7, 0 + 90.5 * 1),
            UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 7, 271.5 + 90.5 * 1),
            UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 7, 362 + 90.5 * 1)

    );
    model.emplace_back(
            0.90,
            UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 4, 90.5 + 90.5 * 2),
            UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 4, 0 + 90.5 * 2),
            UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 4, 271.5 + 90.5 * 2),
            UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 4, 362 + 90.5 * 2)

    );
    model.emplace_back(
            0.91,
            UIVec(-0.5, 0.5), sf::Vector2f(0 + 128, 90.5 + 90.5 * 3),
            UIVec(0.5, 0.5), sf::Vector2f(384 + 128, 0 + 90.5 * 3),
            UIVec(0.5, -0.5), sf::Vector2f(512 + 128, 271.5 + 90.5 * 3),
            UIVec(-0.5, -0.5), sf::Vector2f(128 + 128, 362 + 90.5 * 3)

    );
    model[0].zPosScale = 0.; // shadow stays on the ground
}

Duck::Duck(Game* game) : game(game) {
    // auto shared = std::make_shared<Duck>(this);
    // Entity::childClassPtr = std::dynamic_pointer_cast<Entity>(shared);
    inventory.resize(3, nullptr);
    type = DUCK;
    initModel();
}

Duck::Duck(Game* game, std::shared_ptr<Entity> &entity) : game(game) {
    inventory.resize(3, nullptr);
    id = entity->id;
    type = entity->type;
    position = entity->position;
    deleted = entity->deleted;
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
    ss << "fertilized: " << (fertilized ? "true" : "false") << "\n";
    return ss.str();
}