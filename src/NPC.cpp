#include "entity.h"
#include <iomanip>
#include "player.h"
#include "duck.h"
#include "egg.h"
#include "model.h"
#include "NPC.h"

void npc::runAction(Action& action, std::vector<Action>& followUpActions) {
    switch (action.command) {
    case ON_CREATION:
        // something ... ?
        break;
    }
}

void npc::loadModel() {
    model = &modelNPC;
}

npc::npc(Game* game) : game(game) {
    inventory.resize(1, nullptr);
    type = NPC;
    loadModel();
    collisionPushable = false;
}

void npc::customUpdate() {
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

void npc::setInventoryProps() {
    if (inventory[InventorySlots::LANSEHYAOJI] != nullptr) {
        inventory[InventorySlots::LANSEHYAOJI]->opacity = .1;
    }
}

std::string npc::getDescriptionStr() {
    std::stringstream ss;
    ss << "id: " << id << "\n";
    ss << "type: " << type << "\n";
    ss << "position: " << std::setprecision(3) << std::fixed << position.x << ", " << position.y << "\n";
    ss << "chunk: " << neighborsFinderMyTile.first << ", " << neighborsFinderMyTile.second << "\n";
    ss << "fertilized: " << (fertilized ? "true" : "false") << "\n";
    ss << "gender: " << (genderIsMale ? "male" : "female") << "\n";
    return ss.str();
}