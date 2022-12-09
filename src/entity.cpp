#include "entity.h"
#include <iomanip>

void Entity::runAction(Action& action, std::vector<Action>& followUpActions) {};

void Entity::runActionCommon(Action& action, std::vector<Action>& followUpActions) {
    std::stringstream ss(action.action); 
    std::string function;
    ss >> function;
    if (function == "motion_frozen") {
        bool frozen;
        ss >> frozen;
        motionFrozen = frozen;
    }
    else if (function == "highlightable") {
        bool highlightable;
        ss >> highlightable;
        facingHighlightable = highlightable;
    }
    else if (function == "move_to_instant") {
        coord pos;
        ss >> pos.x >> pos.y;
        position = pos;
    }
    else if (function == "move_to_approach") {
        coord pos;
        double speed;
        ss >> pos.x >> pos.y >> speed;
        position = pos - (pos - position) * std::pow(speed, elapsedSecs);
    }
    else if (function == "heading_instant") {
        double h;
        ss >> h;
        heading = h;
    }
    else if (function == "heading_instant") {
        double h;
        ss >> h;
        heading = h;
    }
    else if (function == "hop") {
        if (zPosition == 0.)
            zVelocity = .2;
    }
    else if (function == "slide_instant") {
        coord delta;
        ss >> delta.x >> delta.y;
        position = position + delta;
    }
    else if (function == "slide_velocity") {
        coord velocity;
        ss >> velocity.x >> velocity.y;
        position = position + velocity / elapsedSecs;
    }
    else if (function == "slide_velocity_distance") {
        coord velocity;
        double distance;
        ss >> velocity.x >> velocity.y >> distance;
        position = position + velocity / velocity.len() * std::min(velocity.len() * elapsedSecs, distance);
        distance -= velocity.len() * elapsedSecs;
        velocity = velocity / velocity.len() * std::max(distance / 0.5, velocity.len() - 0.05 * elapsedSecs); // deccelerate
    }
    else {
        runAction(action, followUpActions);
    }
};

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

}