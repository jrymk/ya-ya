#include "truck.h"
#include "egg.h"
#include <iomanip>
#include "duck.h"
#include "model.h"
#include "localization.h"

void Truck::runAction(Action &action, std::vector<Action> &followUpActions) {
    switch (action.command) {
        case ON_CREATION:
            break;
        case ENTITY_INVENTORY_ON_CAPTURE:
            if (action.argInt[0] >= TRUCK_A0 && action.argInt[0] <= TRUCK_D3) {
                action.argEntity[0]->zPosition = zPosition + .3;
                action.argEntity[0]->selectable = false;
                action.argEntity[0]->collisionPushable = false;
                action.argEntity[0]->motionFrozen = true;
            }
            break;
        case ENTITY_INVENTORY_ON_RELEASE:
            if (action.argInt[0] >= TRUCK_A0 && action.argInt[0] <= TRUCK_D3) {
                action.argEntity[0]->zDepthOverride = -1e8;
                action.argEntity[0]->selectable = true;
            }
            break;
    }
}

void Truck::loadModel() {
    model = &modelTruck;
}

Truck::Truck() { objInit(); }

Truck::Truck(Game* game) : game(game) { objInit(); }

void Truck::objInit() {
    inventory.resize(16, nullptr);
    inventoryPosition.resize(16);
    type = TRUCK;
    footprint = coord(7., 4.);
    collideBox = CollideBox({0., 0.}, {6.8, 3.8}, false);
    collisionNoEnv = true;
    collisionPushable = false;
}

void Truck::customUpdate() {
}

void Truck::setInventoryProps() {
    double truckZDepth = modelTruck[0].zDepth + (Camera::getScreenPos(position).y / Camera::getViewport().size.y - 0.5) / 100.;

    for (int slot = 0; slot < inventory.size(); slot++) {
        inventoryPosition[slot] = {position + coord(-1.5 + 0.8 * (slot % 4 - 1.5), 0. + 0.8 * (slot / 4 - 1.5)), zPosition + 1.5};
        if (!inventory[slot])
            continue;
        if (slot >= TRUCK_A0 && slot <= TRUCK_D3) {
            inventory[slot]->position = inventoryPosition[slot].first;
            inventory[slot]->underlyingPos = inventoryPosition[slot].first;
            inventory[slot]->zPosition = inventoryPosition[slot].second; // do every update or else gravity will do its thing
            inventory[slot]->zVelocity = 0.;
            inventory[slot]->zDepthOverride = truckZDepth + .2; //(slot < 5 ? -ZDEPTH_LAYER : ZDEPTH_LAYER);
        }
    }
}

std::string Truck::getDescriptionStr() {
    std::stringstream ss;
    ss << "id: " << id << "\n";
    ss << "type: " << type << "\n";
    ss << "position: " << std::setprecision(3) << std::fixed << position.x << ", " << position.y << "\n";
    ss << "chunk: " << neighborsFinderMyTile.first << ", " << neighborsFinderMyTile.second << "\n";
    return ss.str();
}

std::wstring Truck::getLocalization(int lang, int strId) {
    return strEggCarton[strId][lang];
}

