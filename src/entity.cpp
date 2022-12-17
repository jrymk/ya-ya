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
            {
                Action a(ownedBy, Timer::getNow(), ENTITY_INVENTORY_ON_CAPTURE);
                a.argEntity[0] = action.entity;
                a.argInt[0] = ownedSlot;
                followUpActions.push_back(a);
            }
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
            {
                Action a(ownedBy, Timer::getNow(), ENTITY_INVENTORY_ON_RELEASE);
                a.argEntity[0] = action.entity;
                a.argInt[0] = ownedSlot;
                followUpActions.push_back(a);
            }
            ownedBy->inventory[ownedSlot] = nullptr;
            ownedBy = nullptr;
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
            underlyingPos = action.argCoord[0] - (action.argCoord[0] - underlyingPos) * std::pow(action.argFloat[0], elapsedSecs * 1000); // not true "time independent" :(
            position = underlyingPos; // TODO: I think this is only used in player inventory???
            break;
        case ENTITY_ZPOS_TO_APPROACH_UNTIL:
            zPosition = action.argFloat[0] - (action.argFloat[0] - zPosition) * std::pow(action.argFloat[1], elapsedSecs);
            if (std::abs(zPosition - action.argFloat[0]) < .01)
                zPosition = action.argFloat[0];
            else {
                Action a(action.entity, Timer::getNow() + 0.01, ENTITY_ZPOS_TO_APPROACH_UNTIL);
                a.argFloat[0] = action.argFloat[0];
                a.argFloat[1] = action.argFloat[1];
                followUpActions.push_back(a);
            }
            break;
        case ENTITY_HEADING_INSTANT:
            heading = action.argFloat[0];
            break;
        case ENTITY_HOP:
            if (zPosition == 0.)
                zVelocity = .1;
            break;
        case ENTITY_SLIDE_INSTANT:
            underlyingPos = underlyingPos + action.argCoord[0];
            position = underlyingPos; // have sex?
            break;
        case ENTITY_SLIDE_VELOCITY: {
            underlyingPos = underlyingPos + action.argCoord[0] / elapsedSecs;
            break;
        }
        case ENTITY_SLIDE_VELOCITY_DISTANCE:
            underlyingPos = underlyingPos + action.argCoord[0] / action.argCoord[0].len() *
                                            std::min(action.argCoord[0].len() * elapsedSecs, action.argFloat[0]);
//            position = underlyingPos;
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
            underlyingPos = underlyingPos + coord::getAngleVec(action.argFloat[0], heading);
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
        quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) * scale + position, zPosition * quad.zPosScale);
        quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) * scale + position, zPosition * quad.zPosScale);
        quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) * scale + position, zPosition * quad.zPosScale);
        quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) * scale + position, zPosition * quad.zPosScale);
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

    motionUpdate();
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

void Entity::motionUpdate() {
    if (!motionFrozen) {
        heading += headingRotationSpeed * elapsedSecs;
        underlyingPos.x += velocity * std::cos(heading) * elapsedSecs;
        underlyingPos.y += velocity * std::sin(heading) * elapsedSecs;
        underlyingPos = underlyingPos + slideVelocity * elapsedSecs;
        if (zPosition > 0)
            zVelocity += GRAVITY * elapsedSecs;
        zPosition += zVelocity;
        zPosition = std::max(zPosition, 0.);
        if (zVelocity != 0 && zPosition == 0.) {
            lastLandTime = Timer::getNow();
            zVelocity = 0.;
        }
    }
    if (!hoppable) /// TODO: fix ducks flying away
        position = underlyingPos;
    else {
        if (position.len(underlyingPos) > .01 && zPosition == 0. && lastLandTime.elapsed() > .15 * hopPower) {
            zVelocity = hopPower;
            _hopVelocity =
                    (underlyingPos - position).unit() * std::min(hopPower * 4.2, position.len(underlyingPos)) /
                    (2. * hopPower / (-GRAVITY)) /* hop time (requires physics knowledge 'o') */;
        }
        if (zPosition > 0.) {
            if (position.len(underlyingPos) < _hopVelocity.len() * elapsedSecs)
                position = underlyingPos;
            else
                position = position + _hopVelocity * elapsedSecs;
            //        position = position + (underlyingPos - position).unit() * std::min(.7, position.len(underlyingPos)) * ;
        }
    }
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

std::wstring Entity::getLocalization(int lang, int strId) {
    return std::wstring(L"general entity");
}


