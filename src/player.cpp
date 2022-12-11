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
    if (inventory[InventorySlots::LEFT_HAND] != nullptr) {
        coord pos = position
                    + coord(Camera::getAngleVectorUntransformed(0.4, heading + PI / 4).x, Camera::getAngleVectorUntransformed(0.4, heading + PI / 4).y);
        {
            Action a(inventory[InventorySlots::LEFT_HAND], Timer::getNow(), ENTITY_MOVE_TO_APPROACH);
            a.argCoord[0] = pos;
            a.argFloat[0] = .00000005;
            game->pushAction(a);
        }
        {
            Action a(inventory[InventorySlots::LEFT_HAND], Timer::getNow(), ENTITY_HEADING_INSTANT);
            a.argFloat[0] = heading;
            game->pushAction(a);
        }
    }
    if (inventory[InventorySlots::RIGHT_HAND] != nullptr) {
        coord pos = position
                    + coord(Camera::getAngleVectorUntransformed(0.4, heading - PI / 4).x, Camera::getAngleVectorUntransformed(0.4, heading - PI / 4).y);
        {
            Action a(inventory[InventorySlots::RIGHT_HAND], Timer::getNow(), ENTITY_MOVE_TO_APPROACH);
            a.argCoord[0] = pos;
            a.argFloat[0] = .00000005;
            game->pushAction(a);
        }
        {
            Action a(inventory[InventorySlots::RIGHT_HAND], Timer::getNow(), ENTITY_HEADING_INSTANT);
            a.argFloat[0] = heading;
            game->pushAction(a);
        }
    }
}

void Player::customUpdate() {
    if (!motionFrozen) {
        heading += headingRotationSpeed * elapsedSecs;
        position.x += velocity * std::cos(heading) * elapsedSecs;
        position.y += velocity * std::sin(heading) * elapsedSecs;
        position = position + slideVelocity * elapsedSecs;
//    debug << "custom updated " << slideVelocity.x << ", " << slideVelocity.y << "\n";
    }
}

