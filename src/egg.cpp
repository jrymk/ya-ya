#include "egg.h"
#include <iomanip>
#include "duck.h"
#include "model.h"
#include "localization.h"

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

                }
                else {
                    double t = 4. + 4. * getRand();
                    {
                        Action a(Timer::getNow() + t, GLOBAL_DESTROY, "egg_" + duck_ptr->id); // DIE
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
        case ENTITY_INVENTORY_ON_CAPTURE:
            if (action.argInt[0] == InventorySlots::EMBRYO) {
                action.argEntity[0]->scale = .3;
                action.argEntity[0]->opacity = .1;
                action.argEntity[0]->selectable = false;
                action.argEntity[0]->collisionCollidable = false;
                action.argEntity[0]->collisionPushable = false;
            }
            break;
        case ENTITY_INVENTORY_ON_RELEASE:
            if (action.argInt[0] == InventorySlots::EMBRYO) {
                action.argEntity[0]->scale = 1.;
                action.argEntity[0]->opacity = 1.;
                action.argEntity[0]->selectable = true;
                action.argEntity[0]->collisionCollidable = true;
                action.argEntity[0]->collisionPushable = true;
            }
            break;
    }
}

void Egg::loadModel() {
    model = &modelEgg[seed % 3];
}

Egg::Egg() { objInit(); }

Egg::Egg(Game* game) : game(game) { objInit(); }

void Egg::objInit() {
    inventory.resize(1, nullptr);
    inventoryPosition.resize(1);
    type = EGG;
    collisionPushable = false;
    collisionCollidable = false;
    footprint = coord(.3, .3);
    collideBox = CollideBox({0., 0.}, {.2, .2}, true);
}

void Egg::customUpdate() {
}

void Egg::setInventoryProps() {
    inventoryPosition[InventorySlots::EMBRYO] = {position, zPosition};
    for (int slot = 0; slot < inventory.size(); slot++) {
        if (!inventory[slot])
            continue;
        if (slot == InventorySlots::EMBRYO) {
            inventory[InventorySlots::EMBRYO]->position = position;
            inventory[InventorySlots::EMBRYO]->underlyingPos = position;
            inventory[InventorySlots::EMBRYO]->zPosition = zPosition;
            inventory[slot]->zVelocity = 0.;
        }
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

std::wstring Egg::getLocalization(int lang, int strId) {
    return strEgg[strId][lang];
}

