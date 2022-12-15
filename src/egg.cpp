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

void Egg::loadModel() {
    model = &modelEgg[seed % 3];
}

Egg::Egg() { objInit(); }

Egg::Egg(Game* game) : game(game) { objInit(); }

void Egg::objInit() {
    inventory.resize(1, nullptr);
    type = EGG;
    collisionPushable = false;
    collisionCollidable = false;
    footprint = coord(.3, .3);
    collideBox = CollideBox({0., 0.}, {.2, .2}, true);
}

void Egg::customUpdate() {
}

void Egg::setInventoryProps() {
    for (int slot = 0; slot < inventory.size(); slot++) {
        if (inventory_last.size() != inventory.size() || !(!inventory_last[slot] && inventory[slot]))
            continue;
        /// ON CAPTURE
        switch (slot) {
            case InventorySlots::EMBRYO:
                inventory[InventorySlots::EMBRYO]->scale = .3;
                inventory[InventorySlots::EMBRYO]->opacity = .1;
                inventory[InventorySlots::EMBRYO]->selectable = false;
                inventory[InventorySlots::EMBRYO]->collisionCollidable = false;
                inventory[InventorySlots::EMBRYO]->collisionPushable = false;
                break;
        }
    }
    for (int slot = 0; slot < inventory.size(); slot++) {
        if (!inventory[slot])
            continue;
        /// ON HOLD
        switch (slot) {
            case InventorySlots::EMBRYO:
                inventory[InventorySlots::EMBRYO]->position = position;
                inventory[InventorySlots::EMBRYO]->underlyingPos = position;
                inventory[InventorySlots::EMBRYO]->zPosition = zPosition;
                break;
        }
    }
    for (int slot = 0; slot < inventory.size(); slot++) {
        if (inventory_last.size() != inventory.size() || !(inventory_last[slot] && !inventory[slot]))
            continue;
        /// ON RELEASE
        switch (slot) {
            case InventorySlots::EMBRYO:
                inventory_last[InventorySlots::EMBRYO]->scale = 1.;
                inventory_last[InventorySlots::EMBRYO]->opacity = 1.;
                inventory_last[InventorySlots::EMBRYO]->selectable = true;
                inventory_last[InventorySlots::EMBRYO]->collisionCollidable = true;
                inventory_last[InventorySlots::EMBRYO]->collisionPushable = true;
                break;
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

