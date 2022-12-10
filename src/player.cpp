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

Player::Player() {
    // auto shared = std::make_shared<Player>(this);
    // Entity::childClassPtr = std::dynamic_pointer_cast<Entity>(shared);
    type = PLAYER;
    initModel();
}

Player::Player(std::shared_ptr<Entity>& entity){
    id = entity -> id;
    type = entity -> type;
    position = entity -> position;
    deleted = entity -> deleted;
    initModel();
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
