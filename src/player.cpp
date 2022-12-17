#include "player.h"
#include <iomanip>
#include "model.h"

void Player::loadModel() {
    model = &modelPlayer;
}

Player::Player() { objInit(); }

Player::Player(Game* game) : game(game) { objInit(); }

void Player::objInit() {
    inventory.resize(2, nullptr);
    inventoryPosition.resize(2);
    footprint = coord(.7, .7);
    collideBox = CollideBox({0., 0.}, {.6, .6}, true);
    type = PLAYER;
    hopPower = .3;
    hoppable = true;
}

void Player::runAction(Action &action, std::vector<Action> &followUpActions) {
    switch (action.command) {
        case ENTITY_INVENTORY_ON_CAPTURE:
            if (action.argInt[0] == InventorySlots::LEFT_HAND || action.argInt[0] == InventorySlots::RIGHT_HAND) {
                action.argEntity[0]->motionFrozen = true;
                action.argEntity[0]->selectable = false;
                {
                    Action a(action.argEntity[0], Timer::getNow(), ENTITY_ZPOS_TO_APPROACH_UNTIL);
                    a.argFloat[0] = 1.;
                    a.argFloat[1] = .00000001;
                    game->pushAction(a);
                }
            }
            break;
        case ENTITY_INVENTORY_ON_RELEASE:
            if (action.argInt[0] == InventorySlots::LEFT_HAND || action.argInt[0] == InventorySlots::RIGHT_HAND) {
                action.argEntity[0]->motionFrozen = false;
                action.argEntity[0]->selectable = true;
            }
            break;
    }
}

void Player::setInventoryProps() {
    inventoryPosition[InventorySlots::LEFT_HAND] = position + coord::getAngleVec(0.5, heading + PI / 4);
    inventoryPosition[InventorySlots::RIGHT_HAND] = position + coord::getAngleVec(0.5, heading + -PI / 4);

    for (int slot = 0; slot < inventory.size(); slot++) {
        if (!inventory[slot])
            continue;
        if (slot == InventorySlots::LEFT_HAND || slot == InventorySlots::RIGHT_HAND) {
//            inventory[slot]->position = pos;
//            inventory[slot]->underlyingPos = pos;
            inventory[slot]->heading = heading;
            {
                Action a(inventory[slot], Timer::getNow(), ENTITY_MOVE_TO_APPROACH);
                a.argCoord[0] = inventoryPosition[slot];
                a.argFloat[0] = 0.90343660158;
                game->pushAction(a);
            }
//
//            {
//                Action a(inventory[slot], Timer::getNow(), ENTITY_HEADING_INSTANT);
//                a.argFloat[0] = heading;
//                game->pushAction(a);
//            }
        }
    }
}

void Player::customUpdate() {

}

void Player::environmentUpdate() {
//    int x = std::floor(position.x);
//    int y = std::floor(position.y);
//    Map::Tile &t = game->map.getTile(x, y);
//    if (t.tileType == Map::Tile::TileType::MOAI) {
//        if (position.x > x + .9 && game->map.getTile(x + 1, y).tileType != Map::Tile::MOAI) {
//            position.x = x + 1;
//        }
//        if (position.x < x + .1 && game->map.getTile(x - 1, y).tileType != Map::Tile::MOAI) {
//            position.x = x;
//        }
//        if (position.y > y + .9 && game->map.getTile(x, y + 1).tileType != Map::Tile::MOAI) {
//            position.y = y + 1;
//        }
//        if (position.y < y + .1 && game->map.getTile(x, y - 1).tileType != Map::Tile::MOAI) {
//            position.y = y;
//        }

//    }


//    for (int i = 0; i < 9; i++) {
//        Map::Tile &t = game->map.getTile(std::floor(position.x) + (i % 3) - 1, std::floor(position.y) + (i / 3) - 1);
//        coord tc = coord(t.x + .5, t.y + .5);
//        if (t.tileType == Map::Tile::TileType::MOAI) {
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
