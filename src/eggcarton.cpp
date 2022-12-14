#include "eggcarton.h"
#include "egg.h"
#include <iomanip>
#include "duck.h"
#include "model.h"

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

void EggCarton::setInventoryProps() {
    double cartonZDepth = modelEggCarton[1].zDepth + (Camera::getScreenPos(position).y / Camera::getViewport().size.y - 0.5) / 100.;

    if (inventory[InventorySlots::EGG_0] != nullptr) {
        inventory[InventorySlots::EGG_0]->position = position + coord(-.3, .075);
        inventory[InventorySlots::EGG_0]->zDepthOverride = cartonZDepth - ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_0]->zPosition = .3;
        inventory[InventorySlots::EGG_0]->selectable = false;
    }
    if (inventory[InventorySlots::EGG_1] != nullptr) {
        inventory[InventorySlots::EGG_1]->position = position + coord(-.15, .075);
        inventory[InventorySlots::EGG_1]->zDepthOverride = cartonZDepth - ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_1]->zPosition = .3;
        inventory[InventorySlots::EGG_1]->selectable = false;
    }
    if (inventory[InventorySlots::EGG_2] != nullptr) {
        inventory[InventorySlots::EGG_2]->position = position + coord(0., .075);
        inventory[InventorySlots::EGG_2]->zDepthOverride = cartonZDepth - ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_2]->zPosition = .3;
        inventory[InventorySlots::EGG_2]->selectable = false;
    }
    if (inventory[InventorySlots::EGG_3] != nullptr) {
        inventory[InventorySlots::EGG_3]->position = position + coord(.15, .075);
        inventory[InventorySlots::EGG_3]->zDepthOverride = cartonZDepth - ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_3]->zPosition = .3;
        inventory[InventorySlots::EGG_3]->selectable = false;
    }
    if (inventory[InventorySlots::EGG_4] != nullptr) {
        inventory[InventorySlots::EGG_4]->position = position + coord(.3, .075);
        inventory[InventorySlots::EGG_4]->zDepthOverride = cartonZDepth - ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_4]->zPosition = .3;
        inventory[InventorySlots::EGG_4]->selectable = false;
    }
    if (inventory[InventorySlots::EGG_5] != nullptr) {
        inventory[InventorySlots::EGG_5]->position = position + coord(-.3, -.075);
        inventory[InventorySlots::EGG_5]->zDepthOverride = cartonZDepth + ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_5]->zPosition = .3;
        inventory[InventorySlots::EGG_5]->selectable = false;
    }
    if (inventory[InventorySlots::EGG_6] != nullptr) {
        inventory[InventorySlots::EGG_6]->position = position + coord(-.15, -.075);
        inventory[InventorySlots::EGG_6]->zDepthOverride = cartonZDepth + ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_6]->zPosition = .3;
        inventory[InventorySlots::EGG_6]->selectable = false;
    }
    if (inventory[InventorySlots::EGG_7] != nullptr) {
        inventory[InventorySlots::EGG_7]->position = position + coord(0., -.075);
        inventory[InventorySlots::EGG_7]->zDepthOverride = cartonZDepth + ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_7]->zPosition = .3;
        inventory[InventorySlots::EGG_7]->selectable = false;
    }
    if (inventory[InventorySlots::EGG_8] != nullptr) {
        inventory[InventorySlots::EGG_8]->position = position + coord(.15, -.075);
        inventory[InventorySlots::EGG_8]->zDepthOverride = cartonZDepth + ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_8]->zPosition = .3;
        inventory[InventorySlots::EGG_8]->selectable = false;
    }
    if (inventory[InventorySlots::EGG_9] != nullptr) {
        inventory[InventorySlots::EGG_9]->position = position + coord(.3, -.075);
        inventory[InventorySlots::EGG_9]->zDepthOverride = cartonZDepth + ZDEPTH_LAYER;
        inventory[InventorySlots::EGG_9]->zPosition = .3;
        inventory[InventorySlots::EGG_9]->selectable = false;
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
