#include "entity.h"
#include <iomanip>
#include "player.h"
#include "duck.h"
#include "egg.h"
#include "model.h"

void Entity::runActionEntity(Action &action, std::vector<Action> &followUpActions) {
    switch (action.command) {
        case ENTITY_OWN_BY:
            ownedBy = action.argEntity[0];
            ownedSlot = action.argInt[0];
            action.argEntity[0]->inventory[action.argInt[0]] = action.entity;
            break;
        case ENTITY_UNOWN: {
            if (ownedBy == nullptr)
                return;
            {
                Action a(action.entity, Timer::getNow(), ON_UNOWNED);
                a.argEntity[0] = ownedBy;
                a.argInt[0] = ownedSlot;
                followUpActions.push_back(a);
            }
            ownedBy->inventory[ownedSlot] = nullptr;
            ownedBy = nullptr;
            opacity = 1.;
            break;
        }
        case ENTITY_MOTION_FROZEN:
            motionFrozen = action.argBool[0];
            break;
        case ENTITY_SELECTABLE:
            selectable = action.argBool[0];
            break;
        case ENTITY_COLLISION_PUSHABLE:
            collisionPushable = action.argBool[0];
            break;
        case ENTITY_COLLISION_COLLIDABLE:
            collisionCollidable = action.argBool[0];
            break;
        case ENTITY_MOVE_TO_INSTANT:
            position = action.argCoord[0];
            break;
        case ENTITY_MOVE_TO_APPROACH:
            position = action.argCoord[0] - (action.argCoord[0] - position) * std::pow(action.argFloat[0], elapsedSecs);
            break;
        case ENTITY_HEADING_INSTANT:
            heading = action.argFloat[0];
            break;
        case ENTITY_HOP:
            if (zPosition == 0.)
                zVelocity = .3;
            break;
        case ENTITY_SLIDE_INSTANT:
            position = position + action.argCoord[0];
            break;
        case ENTITY_SLIDE_VELOCITY: {
            position = position + action.argCoord[0] / elapsedSecs;
            break;
        }
        case ENTITY_SLIDE_VELOCITY_DISTANCE:
            position = position + action.argCoord[0] / action.argCoord[0].len() *
                                  std::min(action.argCoord[0].len() * elapsedSecs, action.argFloat[0]);
            action.argFloat[0] -= action.argCoord[0].len() * elapsedSecs;
            velocity = velocity / action.argCoord[0].len() *
                       std::max(action.argFloat[0] / 0.5, action.argCoord[0].len() - 0.05 * elapsedSecs); // deccelerate
            if (action.argFloat[0] > 0.) {
                Action a(action.entity, Timer::getNow(), ENTITY_SLIDE_VELOCITY_DISTANCE);
                a.argCoord[0] = action.argCoord[0];
                a.argFloat[0] = action.argFloat[0];
                followUpActions.push_back(a);
            }
            break;
        case ENTITY_WALK_INSTANT:
            position = position + coord::getAngleVec(action.argFloat[0], heading);
            break;
        default:
            runAction(action, followUpActions); // run entity specific actions
    }
};

void Entity::loadModel() {
    model = &modelEntity;
}

void Entity::pushQuads() {
    loadModel();
    for (auto quad: *model) {
        quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) * scale + position) +
                  UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) * scale + position) +
                  UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) * scale + position) +
                  UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) * scale + position) +
                  UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.zDepth += (Camera::getScreenPos(position).y / Camera::getViewport().size.y - 0.5) / 100. + zDepthOffset;

        if (zDepthOverride > -100.)
            quad.zDepth = zDepthOverride;

        quad.c0 = sf::Color(255, 255, 255, opacity * 255);
        quad.c1 = sf::Color(255, 255, 255, opacity * 255);
        quad.c2 = sf::Color(255, 255, 255, opacity * 255);
        quad.c3 = sf::Color(255, 255, 255, opacity * 255);

        Graphics::insertQuad(quad);

        if (Graphics::showWireframe) {
            Graphics::insertUserWireframe(
                    Camera::getScreenPos(position),
                    Camera::getScreenPos(position),
                    Camera::getScreenPos(position) + Camera::getAngleVector(0.3, heading),
                    Camera::getScreenPos(position) + Camera::getAngleVector(0.3, heading),
                    sf::Color::Yellow, sf::Color::Black
            );
            Graphics::insertUserWireframe(
                    Camera::getScreenPos(position) + UIVec(5., 0.) + UIVec(0, -zPosition * Camera::getScale()),
                    Camera::getScreenPos(position) + UIVec(0., 5.) + UIVec(0, -zPosition * Camera::getScale()),
                    Camera::getScreenPos(position) + UIVec(-5., 0.) + UIVec(0, -zPosition * Camera::getScale()),
                    Camera::getScreenPos(position) + UIVec(0., -5.) + UIVec(0, -zPosition * Camera::getScale()),
                    sf::Color::Magenta, sf::Color::Black
            );
        }
        if (deleted) {
            Graphics::insertUserWireframe(
                    Camera::getScreenPos(position) + UIVec(-3, 5),
                    Camera::getScreenPos(position) + UIVec(-5, 3),
                    Camera::getScreenPos(position) + UIVec(3, -5),
                    Camera::getScreenPos(position) + UIVec(5, -3),
                    sf::Color::Black, sf::Color::Black
            );
            Graphics::insertUserWireframe(
                    Camera::getScreenPos(position) + UIVec(3, 5),
                    Camera::getScreenPos(position) + UIVec(5, 3),
                    Camera::getScreenPos(position) + UIVec(-3, -5),
                    Camera::getScreenPos(position) + UIVec(-5, -3),
                    sf::Color::Black, sf::Color::Black
            );
            Graphics::insertUserWireframe(
                    Camera::getScreenPos(position) + UIVec(-3, 4),
                    Camera::getScreenPos(position) + UIVec(-4, 3),
                    Camera::getScreenPos(position) + UIVec(3, -4),
                    Camera::getScreenPos(position) + UIVec(4, -3),
                    sf::Color::Red, sf::Color::Transparent
            );
            Graphics::insertUserWireframe(
                    Camera::getScreenPos(position) + UIVec(3, 4),
                    Camera::getScreenPos(position) + UIVec(4, 3),
                    Camera::getScreenPos(position) + UIVec(-3, -4),
                    Camera::getScreenPos(position) + UIVec(-4, -3),
                    sf::Color::Red, sf::Color::Transparent
            );
        }
    }
}

void Entity::updateTimer() {
    double thisUpdate = entityTimer.elapsed();
    if (lastUpdate >= 0) {
        elapsedSecs = thisUpdate - lastUpdate;
    }
    lastUpdate = thisUpdate;
}

void Entity::update() {
    updateTimer();
    historyPosition.emplace_front(Timer::getNow(), position);
    while (!historyPosition.empty() && historyPosition.front().first.elapsed() >= 1.0)
        historyPosition.pop_back();

    inventory_last = inventory;
    customUpdate();
    environmentUpdate();
}

void Entity::environmentUpdate() {}

std::string Entity::getDescriptionStr() {
    std::stringstream ss;
    ss << "id: " << id << "\n";
    ss << "type: " << type << "\n";
    ss << "position: " << std::setprecision(3) << std::fixed << position.x << ", " << position.y << "\n";
    ss << "chunk: " << neighborsFinderMyTile.first << ", " << neighborsFinderMyTile.second << "\n";
    return ss.str();
}

void Entity::customUpdate() {
    debug << "Entity version of custom update is ran\n";
}

Entity::~Entity() {
//    debug << "Entity destroyed\n";
}

void Entity::runAction(Action &action, std::vector<Action> &followUpActions) {

}

void Entity::setInventoryProps() {

}

void Entity::objInit() {

}

Entity::Entity() {
    seed = getRandInt();
}

