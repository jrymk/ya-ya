#include "player.h"
#include <iomanip>

void Player::initModel() {
    model.push_back(
            Graphics::Quad(
                    0.80,
                    UIVec(0.5, -0.5), sf::Vector2f(0 + 128 * 7, 90.5 + 90.5 * 1),
                    UIVec(-0.5, -0.5), sf::Vector2f(384 + 128 * 7, 0 + 90.5 * 1),
                    UIVec(-0.5, 0.5), sf::Vector2f(512 + 128 * 7, 271.5 + 90.5 * 1),
                    UIVec(0.5, 0.5), sf::Vector2f(128 + 128 * 7, 362 + 90.5 * 1)
            )
    );
    model.push_back(
            Graphics::Quad(
                    0.91,
                    UIVec(-0.5, 1.5), sf::Vector2f(0 + 128 * 2, 90.5 + 90.5 * 6),
                    UIVec(0.5, 1.5), sf::Vector2f(384 + 128 * 2, 0 + 90.5 * 6),
                    UIVec(0.5, 0.5), sf::Vector2f(512 + 128 * 2, 271.5 + 90.5 * 6),
                    UIVec(-0.5, 0.5), sf::Vector2f(128 + 128 * 2, 362 + 90.5 * 6)
            )
    );
    model.push_back(
            Graphics::Quad(
                    0.91,
                    UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 3, 90.5 + 90.5 * 9),
                    UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 3, 0 + 90.5 * 9),
                    UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 3, 271.5 + 90.5 * 9),
                    UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 3, 362 + 90.5 * 9)
            )
    );
}

Player::Player(Game* game) : game(game) {
    // auto shared = std::make_shared<Player>(this);
    // Entity::childClassPtr = std::dynamic_pointer_cast<Entity>(shared);
    inventory.resize(2, nullptr);
    type = PLAYER;
    initModel();
}

Player::Player(Game* game, std::shared_ptr<Entity> &entity) : game(game) {
    inventory.resize(2, nullptr);
    id = entity->id;
    type = entity->type;
    position = entity->position;
    deleted = entity->deleted;
    initModel();
}

void Player::setInventoryProps() {
    if (inventory.size() == inventory_last.size()) {
        for (int slot = 0; slot < inventory.size(); slot++) {
            if (!(!inventory_last[slot] && inventory[slot]))
                continue;
            /// ON CAPTURE
            switch (slot) {
                case InventorySlots::LEFT_HAND:
                case InventorySlots::RIGHT_HAND:
                    inventory[slot]->motionFrozen = true;
                    inventory[slot]->selectable = false;
                    break;
            }
        }
    }
    for (int slot = 0; slot < inventory.size(); slot++) {
        if (!inventory[slot])
            continue;
        /// ON HOLD
        switch (slot) {
            case InventorySlots::LEFT_HAND:
            case InventorySlots::RIGHT_HAND:
                coord pos = position + coord::getAngleVec(0.4, heading + (slot == InventorySlots::LEFT_HAND ? PI / 4 : -PI / 4));
                {
                    Action a(inventory[slot], Timer::getNow(), ENTITY_MOVE_TO_APPROACH);
                    a.argCoord[0] = pos;
                    a.argFloat[0] = .0000000005;
                    game->pushAction(a);
                }
                {
                    Action a(inventory[slot], Timer::getNow(), ENTITY_HEADING_INSTANT);
                    a.argFloat[0] = heading;
                    game->pushAction(a);
                }
                break;
        }
    }
    if (inventory.size() == inventory_last.size()) {
        for (int slot = 0; slot < inventory.size(); slot++) {
            if (!(inventory_last[slot] && !inventory[slot]))
                continue;
            /// ON RELEASE
            switch (slot) {
                case InventorySlots::LEFT_HAND:
                case InventorySlots::RIGHT_HAND:
                    inventory_last[slot]->motionFrozen = false;
                    inventory_last[slot]->selectable = true;
                    break;
            }
        }
    }

    inventory_last = inventory;
}

void Player::customUpdate() {
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
//    debug << "custom updated " << slideVelocity.x << ", " << slideVelocity.y << "\n";
    }
}
