#include "entity.h"
#include <iomanip>
#include "player.h"
#include "duck.h"
#include "egg.h"

void Entity::runAction(Action& action, std::vector<Action>& followUpActions) {};

void Entity::initModel() {}

void Entity::pushQuads() {
    for (auto quad : model) {
        quad.v0 = Camera::getScreenPos(coord(quad.v0.x, quad.v0.y) + position) + UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.v1 = Camera::getScreenPos(coord(quad.v1.x, quad.v1.y) + position) + UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.v2 = Camera::getScreenPos(coord(quad.v2.x, quad.v2.y) + position) + UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
        quad.v3 = Camera::getScreenPos(coord(quad.v3.x, quad.v3.y) + position) + UIVec(0, -zPosition * quad.zPosScale * Camera::getScale());
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
    historyPosition.push_front({Timer::getNow(), position});
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
    debug << "Entity destroyed\n";
}