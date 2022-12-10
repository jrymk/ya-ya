#include "entity.h"
#include <iomanip>
#include "player.h"
#include "duck.h"
#include "egg.h"

void Entity::runActionEntity(Action &action, std::vector<Action> &followUpActions) {
    switch (action.command) {
        case ENTITY_MOTION_FROZEN:
            motionFrozen = action.argBool[0];
            break;
        case ENTITY_HIGHLIGHTABLE:
            facingHighlightable = action.argBool[0];
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
                zVelocity = .2;
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
            position = position + coord(Camera::getAngleVectorUntransformed(action.argFloat[0], heading).x,
                                        Camera::getAngleVectorUntransformed(action.argFloat[0], heading).y);
            break;
        default:
            runAction(action, followUpActions); // run entity specific actions
    }
};

void Entity::initModel() {}

void Entity::pushQuads() {
    for (auto quad: model) {
        quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) + position) +
                  UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) + position) +
                  UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) + position) +
                  UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) + position) +
                  UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.zDepth += (Camera::getScreenPos(position).y / Camera::getViewport().size.y - 0.5) / 100.;
        // quad.c0 = genderIsMale ? sf::Color(230, 230, 255) : sf::Color(255, 230, 230);
        // quad.c1 = genderIsMale ? sf::Color(230, 230, 255) : sf::Color(255, 230, 230);
        // quad.c2 = genderIsMale ? sf::Color(230, 230, 255) : sf::Color(255, 230, 230);
        // quad.c3 = genderIsMale ? sf::Color(230, 230, 255) : sf::Color(255, 230, 230);
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


    customUpdate();
}

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

