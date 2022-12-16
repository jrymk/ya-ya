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
    }
}

void EggCarton::loadModel() {
    model = &modelEggCarton;
}

EggCarton::EggCarton() { objInit(); }

EggCarton::EggCarton(Game* game) : game(game) { objInit(); }

void EggCarton::objInit() {
    inventory.resize(10, nullptr);
    type = EGG_CARTON;
    footprint = coord(1., .5);
    collideBox = CollideBox({0., 0.}, {.9, .4}, false);
}

void EggCarton::customUpdate() {
}

void EggCarton::setInventoryProps() {
    double cartonZDepth = modelEggCarton[1].zDepth + (Camera::getScreenPos(position).y / Camera::getViewport().size.y - 0.5) / 100.;

    for (int slot = 0; slot < inventory.size(); slot++) {
        if (!((inventory_last.size() != inventory.size() || !inventory_last[slot]) && inventory[slot]))
            continue;
        /// ON CAPTURE
        if (slot >= EGG_0 && slot <= EGG_9) {
            inventory[slot]->zPosition = zPosition + .3;
            inventory[slot]->selectable = false;
            // eggs are not pushable anyways
        }
    }

    for (int slot = 0; slot < inventory.size(); slot++) {
        if (!inventory[slot])
            continue;
        /// ON HOLD
        if (slot >= EGG_0 && slot <= EGG_9) {
            inventory[slot]->position = position + coord(-.3 + (slot % 5) * .15, (slot < 5 ? .075 : -.075));
            inventory[slot]->underlyingPos = position + coord(-.3 + (slot % 5) * .15, (slot < 5 ? .075 : -.075));
            inventory[slot]->zPosition = zPosition + .3; // do every update or else gravity will do its thing
            inventory[slot]->zDepthOverride = cartonZDepth + (slot < 5 ? -ZDEPTH_LAYER : ZDEPTH_LAYER);
        }
    }
    for (int slot = 0; slot < inventory.size(); slot++) {
        if (inventory_last.size() != inventory.size() || !(inventory_last[slot] && !inventory[slot]))
            continue;
        /// ON RELEASE
        if (slot >= EGG_0 && slot <= EGG_9) {
            inventory_last[slot]->zDepthOverride = -1e8;
            inventory_last[slot]->selectable = true;
        }
    }
    inventory_last = inventory;
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

