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

    for (int i = 0; i < 10; i++) {
        if (inventory[i] == nullptr)
            continue;
        inventory[i]->position = position + coord(-.3 + (i % 5) * .15, (i < 5 ? .075 : -.075));
        inventory[i]->zDepthOverride = cartonZDepth + (i < 5 ? -ZDEPTH_LAYER : ZDEPTH_LAYER);
        inventory[i]->zPosition = .3;
        inventory[i]->selectable = false;
        inventory[i]->collisionCollidable = false;
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
