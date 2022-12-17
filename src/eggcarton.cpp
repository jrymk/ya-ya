#include "eggcarton.h"
#include "egg.h"
#include <iomanip>
#include "duck.h"
#include "model.h"
#include "localization.h"

void EggCarton::runAction(Action &action, std::vector<Action> &followUpActions) {
    switch (action.command) {
        case ON_CREATION:
            break;
        case ENTITY_INVENTORY_ON_CAPTURE:
            if (action.argInt[0] >= EGG_0 && action.argInt[0] <= EGG_9) {
                action.argEntity[0]->zPosition = zPosition + .3;
                action.argEntity[0]->selectable = false;
                // eggs are not pushable anyways
            }
            break;
        case ENTITY_INVENTORY_ON_RELEASE:
            if (action.argInt[0] >= EGG_0 && action.argInt[0] <= EGG_9) {
                action.argEntity[0]->zDepthOverride = -1e8;
                action.argEntity[0]->selectable = true;
            }
            break;
    }
}

void EggCarton::loadModel() {
    model = &modelEggCarton;
}

EggCarton::EggCarton() { objInit(); }

EggCarton::EggCarton(Game* game) : game(game) { objInit(); }

void EggCarton::objInit() {
    inventory.resize(10, nullptr);
    inventoryPosition.resize(10);
    type = EGG_CARTON;
    footprint = coord(1., .5);
    collideBox = CollideBox({0., 0.}, {.9, .4}, false);
}

void EggCarton::customUpdate() {
}

void EggCarton::setInventoryProps() {
    double cartonZDepth = zDepthOverride > -100 ? zDepthOverride : modelEggCarton[1].zDepth + (Camera::getScreenPos(position).y / Camera::getViewport().size.y - 0.5) / 100.;

    for (int slot = 0; slot < inventory.size(); slot++) {
        inventoryPosition[slot] = {position + coord(-.3 + (slot % 5) * .15, (slot < 5 ? .075 : -.075)), zPosition + .12};
        if (!inventory[slot])
            continue;
        if (slot >= EGG_0 && slot <= EGG_9) {
            inventory[slot]->position = inventoryPosition[slot].first;
            inventory[slot]->underlyingPos = inventoryPosition[slot].first;
            inventory[slot]->zPosition = inventoryPosition[slot].second; // do every update or else gravity will do its thing
            inventory[slot]->zVelocity = 0.;
            inventory[slot]->zDepthOverride = cartonZDepth + (slot < 5 ? -ZDEPTH_LAYER : ZDEPTH_LAYER);
        }
    }
}

std::string EggCarton::getDescriptionStr() {
    std::stringstream ss;
    ss << "id: " << id << "\n";
    ss << "type: " << type << "\n";
    ss << "position: " << std::setprecision(3) << std::fixed << position.x << ", " << position.y << "\n";
    ss << "chunk: " << neighborsFinderMyTile.first << ", " << neighborsFinderMyTile.second << "\n";
    return ss.str();
}

std::wstring EggCarton::getLocalization(int lang, int strId) {
    return strEggCarton[strId][lang];
}

