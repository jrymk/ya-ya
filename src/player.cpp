#include "player.h"
#include <iomanip>
#include "model.h"

void Player::loadModel() {
    model = &modelPlayer;
}

Player::Player(Game* game) : game(game) {
    // auto shared = std::make_shared<Player>(this);
    // Entity::childClassPtr = std::dynamic_pointer_cast<Entity>(shared);
    inventory.resize(2, nullptr);
    type = PLAYER;
    loadModel();
}

Player::Player(Game* game, std::shared_ptr<Entity> &entity) : game(game) {
    inventory.resize(2, nullptr);
    id = entity->id;
    type = entity->type;
    position = entity->position;
    deleted = entity->deleted;
    loadModel();
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

void Player::environmentUpdate() {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    Map::Tile &t = game->map.getTile(x, y);
    if (t.tileType == Map::Tile::TileType::STONE) {
        if (position.x > x + .9 && game->map.getTile(x + 1, y).tileType != Map::Tile::STONE) {
            position.x = x + 1;
        }
        if (position.x < x + .1 && game->map.getTile(x - 1, y).tileType != Map::Tile::STONE) {
            position.x = x;
        }
        if (position.y > y + .9 && game->map.getTile(x, y + 1).tileType != Map::Tile::STONE) {
            position.y = y + 1;
        }
        if (position.y < y + .1 && game->map.getTile(x, y - 1).tileType != Map::Tile::STONE) {
            position.y = y;
        }

    }


//    for (int i = 0; i < 9; i++) {
//        Map::Tile &t = game->map.getTile(std::floor(position.x) + (i % 3) - 1, std::floor(position.y) + (i / 3) - 1);
//        coord tc = coord(t.x + .5, t.y + .5);
//        if (t.tileType == Map::Tile::TileType::STONE) {
////            if (position.x > t.x && position.x < t.x + .5)
////                position.x = t.x;
////            if (position.x < t.x + 1 && position.x > t.x + .5)
////                position.x = t.x + 1;
////            if (position.y > t.y && position.y < t.y + .5)
////                position.y = t.y;
////            if (position.y < t.y + 1 && position.y > t.y + .5)
////                position.y = t.y + 1;
//            if (position.len(tc) < .6) {
//                position = position + (tc - position).unit() * ((tc - position).len() - .6);
//            }
//        }
//    }
}
