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
            if (action.argInt[0] >= TRUCK_0 && action.argInt[0] <= TRUCK_9) {
                action.argEntity[0]->zPosition = zPosition + .3;
                action.argEntity[0]->selectable = false;
                // eggs are not pushable anyways
            }
            break;
        case ENTITY_INVENTORY_ON_RELEASE:
            if (action.argInt[0] >= TRUCK_0 && action.argInt[0] <= TRUCK_9) {
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
    inventory.resize(10, nullptr);
    inventoryPosition.resize(10);
    type = TRUCK;
    footprint = coord(7., 4.);
    collideBox = CollideBox({0., 0.}, {6.8, 3.8}, false);
}

void Truck::customUpdate() {
}

void Truck::setInventoryProps() {
    double cartonZDepth = modelTruck[1].zDepth + (Camera::getScreenPos(position).y / Camera::getViewport().size.y - 0.5) / 100.;

    for (int slot = 0; slot < inventory.size(); slot++) {
        inventoryPosition[slot] = position + coord(-.3 + (slot % 5) * .15, (slot < 5 ? .075 : -.075));
        if (!inventory[slot])
            continue;
        if (slot >= TRUCK_0 && slot <= TRUCK_9) {
            inventory[slot]->position = inventoryPosition[slot];
            inventory[slot]->underlyingPos = inventoryPosition[slot];
            inventory[slot]->zPosition = zPosition + .3; // do every update or else gravity will do its thing
            inventory[slot]->zDepthOverride = cartonZDepth + (slot < 5 ? -ZDEPTH_LAYER : ZDEPTH_LAYER);
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

