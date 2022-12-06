#include "egg.h"
#include <iomanip>

void Egg::runAction(Action& action, std::vector<Action>& followUpActions) {
    switch (action.command) {
    case INIT: {
        if (!fertilized) {
            Action a(nullptr, Timer::getNow() + 10., GLOBAL_DESTROY);
            a.argString[0] = id;
            followUpActions.push_back(a);
        }
        break;
    }
    case EGG_HATCH: {
        Action a(nullptr, Timer::getNow(), GLOBAL_HATCH);
        a.argCoord[0] = position;
        a.argBool[0] = genderIsMale;
        followUpActions.push_back(a);
        Action b(nullptr, Timer::getNow() + 1., GLOBAL_DESTROY);
        b.argString[0] = id;
        followUpActions.push_back(b);
            // delete this;
        break;
    }
    case EGG_HOP: {
        if (zPosition == 0.)
            zVelocity = .2;
        break;
    }
    }
}

void Egg::initModel() {
    model.push_back(
        Graphics::Quad(
            0.85,
            UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 10, 90.5 + 90.5 * 0), 
            UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 10, 0 + 90.5 * 0), 
            UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 10, 271.5 + 90.5 * 0),
            UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 10, 362 + 90.5 * 0)
        )
    );
}

Egg::Egg() {
    Entity::childClassPtr = this;
    type = EGG;
    initModel();
}

void Egg::customUpdate() {
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

std::string Egg::getDescriptionStr() {
    std::stringstream ss;
    ss << "id: " << id << "\n";
    ss << "type: " << type << "\n";
    ss << "position: " << std::setprecision(3) << std::fixed << position.x << ", " << position.y << "\n";
    ss << "chunk: " << neighborsFinderMyTile.first << ", " << neighborsFinderMyTile.second << "\n";
    ss << "fertilized: " << (fertilized ? "true" : "false") << "\n";
    ss << "gender: " << (genderIsMale ? "male" : "female") << "\n";
    return ss.str();
}