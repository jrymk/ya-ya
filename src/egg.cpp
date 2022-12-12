#include "egg.h"
#include <iomanip>
#include "duck.h"

void Egg::runAction(Action &action, std::vector<Action> &followUpActions) {
    switch (action.command) {
        case ON_CREATION:
            if (fertilized)
                followUpActions.push_back(Action(action.entity, Timer::getNow() + 3. + getRand() * 10., EGG_FORM_EMBRYO));
            break;
        case ON_UNOWNED: // pop out of duck's ass
            followUpActions.push_back(Action(action.entity, Timer::getNow(), ENTITY_HOP));
            opacity = 1.;
            break;
        case EGG_FORM_EMBRYO:
            if (getRand() > .9) { // 90% success
                fertilized = false;
                followUpActions.push_back(Action(action.entity, Timer::getNow(), ON_CREATION)); // be an unfertilized egg I guess (reinit)
                return;
            }
            {
                std::shared_ptr<Duck> duck(new Duck(game)); // this is a duck that shouldn't look like a duck yet at all
                duck->id = game->newId(DUCK);
//                duck->opacity = 1.;
                duck->position = position + coord::getRandCoord();
                duck->genderIsMale = genderIsMale;
                duck->selectable = false;
                duck->collisionPushable = false;
                duck->collisionCollidable = false;
                auto &duck_ptr = game->insertEntity(duck, true); // it will be the creator's responsibility to suppress init if the entity is being pregenerated
                {
                    Action a(duck_ptr, Timer::getNow(), ENTITY_OWN_BY);
                    a.argEntity[0] = action.entity;
                    a.argInt[0] = InventorySlots::EMBRYO;
                    followUpActions.push_back(a);
                }
                if (getRand() < .8) { // 80% success
                    double t = 3. + 4. * getRand();
                    followUpActions.emplace_back(duck_ptr, Timer::getNow() + t, ENTITY_UNOWN); // HATCH
                    followUpActions.emplace_back(duck_ptr, Timer::getNow() + t, ON_CREATION);
                    {
                        Action a(Timer::getNow() + t + 2., GLOBAL_DESTROY); // destroy egg
                        a.argEntity[0] = action.entity;
                        followUpActions.push_back(a);
                    }

                } else {
                    double t = 4. + 4. * getRand();
                    {
                        Action a(Timer::getNow() + t, GLOBAL_DESTROY, "egg " + duck_ptr->id); // DIE
                        a.argEntity[0] = duck_ptr;
                        followUpActions.push_back(a);
                    }
                    {
                        Action a(Timer::getNow() + t + 2., GLOBAL_DESTROY); // destroy egg
                        a.argEntity[0] = action.entity;
                        followUpActions.push_back(a);
                    }
                }
            }
            break;
    }
}

void Egg::initModel() {
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

Egg::Egg(Game* game) : game(game) {
    // auto shared = std::make_shared<Egg>(this);
    inventory.resize(1, nullptr);
    type = EGG;
    initModel();
    collisionPushable = false;
}

void Egg::customUpdate() {
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

void Egg::setInventoryProps() {
    if (inventory[InventorySlots::EMBRYO] != nullptr) {
        inventory[InventorySlots::EMBRYO]->opacity = .1;
    }
}

std::string Egg::getDescriptionStr() {
    std::stringstream ss;
    ss << "id: " << id << "\n";
    ss << "type: " << type << "\n";
    ss << "position: " << std::setprecision(3) << std::fixed << position.x << ", " << position.y << "\n";
    ss << "chunk: " << neighborsFinderMyTile.first << ", " << neighborsFinderMyTile.second << "\n";
    ss << "fertilized: " << (fertilized ? "true" : "false") << "\n";
    ss << "gender: " << (genderIsMale ? "male" : "female") << "\n";
    return ss.str();
}
