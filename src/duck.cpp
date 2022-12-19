#include "duck.h"
#include <iomanip>
#include "game.h"
#include "egg.h"
#include "model.h"
#include "localization.h"

void Duck::runAction(Action &action, std::vector<Action> &followUpActions) {
    switch (action.command) {
        case ON_CREATION: { // actual creation
            followUpActions.emplace_back(action.entity, Timer::getNow() + 5. + getRand() * 25., DUCK_LOOP_WANDER);
            double growUpTime = 40. + 120. * getRand();
            followUpActions.emplace_back(action.entity, Timer::getNow() + growUpTime * .2, DUCK_GROW);
            followUpActions.emplace_back(action.entity, Timer::getNow() + growUpTime, DUCK_GROW);

            if (genderIsMale) {
                followUpActions.emplace_back(action.entity, Timer::getNow() + growUpTime * .9 + getRand() * 20., DUCK_LOOP_FIND_MATE);
            }
            else {
                followUpActions.emplace_back(action.entity, Timer::getNow() + growUpTime + getRand() * 40., DUCK_LOOP_LAY_EGGS);
            }

            followUpActions.emplace_back(action.entity, Timer::getNow(), ENTITY_HOP);
            followUpActions.emplace_back(action.entity, Timer::getNow() + 120. + getRand() * 360., DUCK_DEATH, "creation_" + action.entity->id);

            {
                Action a(action.entity, Timer::getNow() + .3, DUCK_DUCKWALK_TO_UNTIL);
                a.argCoord[0].x = position.x + 3. * (getRand() - 0.5);
                a.argCoord[0].y = position.y + 3. * (getRand() - 0.5);
                a.argFloat[0] = 5.;
                followUpActions.push_back(a);
            }
            opacity = 1.; // this is actual creation
            selectable = true;
            collisionPushable = true;
            collisionCollidable = true;
            break;
        }
        case ON_UNOWNED:

            break;
        case DUCK_GROW:
            if (growStage != GROWN)
                growStage = static_cast<GrowStage>(growStage + 1);
        case DUCK_LOOP_WANDER: {
            followUpActions.emplace_back(action.entity, Timer::getNow() + 5. + getRand() * 25., DUCK_LOOP_WANDER);
            Action a(action.entity, Timer::getNow(), DUCK_DUCKWALK_TO_UNTIL);
            a.argCoord[0].x = position.x + getRand() * 3.;
            a.argCoord[0].y = position.y + getRand() * 3.;
            a.argFloat[0] = 15.;
            followUpActions.push_back(a);
            break;
        }
        case DUCK_LOOP_LAY_EGGS: { // female only
            if (fertilized) {
                if (inventory[InventorySlots::EGG_0] == nullptr) {
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

                if (getRand() < .1 && inventory[InventorySlots::EGG_1] == nullptr) {// 10% a twin!
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
            }
            else {
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
                        Action a(action.entity, Timer::getNow() + .1, DUCK_DUCKWALK_TO_DUCK);
                        a.argEntity[0] = closest;
                        a.argFloat[0] = 15.;
                        followUpActions.push_back(a);
                    }
                    {
                        Action a(action.entity, Timer::getNow() + .1, DUCK_UNTIL_MATE_CONTACT);
                        a.argEntity[0] = closest;
                        followUpActions.push_back(a);
                    }
                }
            }
            followUpActions.emplace_back(action.entity, Timer::getNow() + 5. + getRand() * 20., DUCK_LOOP_FIND_MATE);
            break;
        }
        case DUCK_UNTIL_MATE_CONTACT: {
            if (position.len(action.argEntity[0]->position) < .35) { // have contact (minimal sexage distance)
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
            }
            else {
                Action a(action.entity, Timer::getNow() + .1, DUCK_UNTIL_MATE_CONTACT);
                a.argEntity[0] = action.argEntity[0];
                followUpActions.push_back(a);
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
                Action a(Timer::getNow() + .5, GLOBAL_DESTROY, "death " + action.entity->id);
                a.argEntity[0] = action.entity;
                followUpActions.push_back(a);
            }
            break;
        }
        case DUCK_HAVE_SEX_WITH: {
            motionFrozen = true;
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
            }
            else {
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
            std::srand(std::time(0));
            duckAudio.playSound(std::rand() % 2);
            if (!genderIsMale) {
                if (getRand() < .9) // 90% success sex
                    fertilized = true;
            }
            else {
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
            }
            else {
                heading = position.angle(action.argCoord[0]);
                velocity = 3. + (getRand() * 2. - 1.);
            }

            if (position.len(action.argCoord[0]) < .7) {
                velocity = 0.;
                headingRotationSpeed = 0.;
            }
            else if (action.argFloat[0] > 0.) {
                Timer t = Timer::getNow() + 0.2 * std::sqrt(position.len(action.argCoord[0]));
                Action a(action.entity, t, DUCK_DUCKWALK_TO_UNTIL);
                a.argCoord[0] = action.argCoord[0];
                a.argFloat[0] = t.elapsed(action.time + action.argFloat[0]);
                followUpActions.push_back(a);
            }
            break;
        }
        case DUCK_DUCKWALK_TO_DUCK: {
            if (position.len(action.argEntity[0]->position) > 2.) {
                coord randTarget = action.argEntity[0]->position;
                if (std::abs(subtractAngle(heading, position.angle(randTarget))) > 1.)
                    heading = position.angle(randTarget) + .2 * (getRand() * .4 - .2);
                headingRotationSpeed = -0.2 * subtractAngle(heading, position.angle(randTarget)) + (getRand() * .1 - .05);
                velocity = 4. + (getRand() * 2. - 1.);
            }
            else {
                heading = position.angle(action.argEntity[0]->position);
                velocity = 3. + (getRand() * 2. - 1.);
            }

            if (position.len(action.argEntity[0]->position) < .3) {
                velocity = 0.;
                headingRotationSpeed = 0.;
            }
            else if (action.argFloat[0] > 0.) {
                Timer t = Timer::getNow() + 0.2 * std::sqrt(position.len(action.argEntity[0]->position));
                Action a(action.entity, t, DUCK_DUCKWALK_TO_DUCK);
                a.argEntity[0] = action.argEntity[0];
                a.argFloat[0] = t.elapsed(action.time + action.argFloat[0]);
                followUpActions.push_back(a);
            }
            break;
        }
        case ENTITY_INVENTORY_ON_CAPTURE:
            if (action.argInt[0] == InventorySlots::EGG_0 || action.argInt[0] == InventorySlots::EGG_1) {
                action.argEntity[0]->motionFrozen = true;
                action.argEntity[0]->selectable = false;
                action.argEntity[0]->opacity = .1;
                action.argEntity[0]->scale = .6;
            }
            break;
        case ENTITY_INVENTORY_ON_RELEASE:
            if (action.argInt[0] == InventorySlots::EGG_0 || action.argInt[0] == InventorySlots::EGG_1) {
                action.argEntity[0]->motionFrozen = false;
                action.argEntity[0]->selectable = true;
                action.argEntity[0]->opacity = 1.;
                action.argEntity[0]->scale = 1.;
            }
            break;
    }
}

void Duck::loadModel() {
    duckModel.clear();
    switch (growStage) {
        case DUCKLING:
            duckModel.push_back(modelDuckling);
            break;
        case CHILD:
            duckModel.push_back(modelChildDuck);
            break;
        case GROWN:
        default:
            duckModel.push_back(modelGrownDuck);
            break;
    }
    int modelId = int((-heading * 10. / PI) + 10 + 20.5) % 20;
    UIVec xDelta = UIVec(duckModel[0].t2 - duckModel[0].t3) / 5.;
    UIVec yDelta = UIVec(duckModel[0].t0 - duckModel[0].t3) / 4.;
    duckModel[0].t0 = (UIVec(duckModel[0].t3) + xDelta * (modelId % 5 + 0) + yDelta * (3 - modelId / 5 % 4 + 1)).getVec2f();
    duckModel[0].t1 = (UIVec(duckModel[0].t3) + xDelta * (modelId % 5 + 1) + yDelta * (3 - modelId / 5 % 4 + 1)).getVec2f();
    duckModel[0].t2 = (UIVec(duckModel[0].t3) + xDelta * (modelId % 5 + 1) + yDelta * (3 - modelId / 5 % 4 + 0)).getVec2f();
    duckModel[0].t3 = (UIVec(duckModel[0].t3) + xDelta * (modelId % 5 + 0) + yDelta * (3 - modelId / 5 % 4 + 0)).getVec2f();

    model = &duckModel;
}

Duck::Duck() {
    objInit();
}

Duck::Duck(Game* game) : game(game) {
    objInit();
}

void Duck::objInit() {
    growStage = DUCKLING;
    inventory.resize(3, nullptr);
    inventoryPosition.resize(3);
    type = DUCK;
    footprint = coord(.5, .5);
    collideBox = CollideBox({0., 0.}, {.4, .4}, true);
    hopPower = .08;
    hoppable = true;
    duckAudio.setVolume(1);
    duckAudio.loadSound("./res/quack-1.wav");
    duckAudio.setVolume(1);
    duckAudio.loadSound("./res/quack-2.wav");

}

void Duck::customUpdate() {
}

void Duck::setInventoryProps() {
    inventoryPosition[InventorySlots::EGG_0] = {position + coord::getAngleVec(-.1, heading), zPosition};
    inventoryPosition[InventorySlots::EGG_1] = {position + coord::getAngleVec(-.2, heading), zPosition};
    for (int slot = 0; slot < inventory.size(); slot++) {
        if (!inventory[slot])
            continue;
        if (slot == InventorySlots::EGG_0 || slot == InventorySlots::EGG_1) {
            inventory[slot]->position = inventoryPosition[slot].first;
            inventory[slot]->underlyingPos = inventoryPosition[slot].first;
            inventory[slot]->heading = heading;
            inventory[slot]->zPosition = inventoryPosition[slot].second;
        }
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

void Duck::environmentUpdate() {
//    for (int i = 0; i < 9; i++) {
//        Map::Tile &t = game->map.getTile(std::floor(position.x) + (i % 3) - 1, std::floor(position.y) + (i / 3) - 1);
//        if (t.tileType == Map::Tile::TileType::MOAI) {
//            if (position.len(coord(t.x, t.y)) < .5) {
//                position = position + (coord(t.x, t.y) - position).unit() * ((coord(t.x, t.y) - position).len() - .5);
//            }
//        }
//    }
}

std::wstring Duck::getLocalization(int lang, int strId) {
    return strDuck[strId][lang];
}

