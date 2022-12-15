#include "ui.h"
#include "game.h"
#include "graphics.h"
#include "camera.h"
#include "entity.h"
#include "player.h"

void UserInterface::renderUI() {
    Graphics::
    Graphics::setFont(3);
    const float totalHeight = 120.;
    const float bottomPadding = 8.;
    const float padding = 12.;
    const float textLRPadding = 4.;

    UIRect bottomBarRect;
    bottomBarRect.pos = Camera::getViewport() * UIVec(0., 1.) + UIVec(0., -totalHeight);
    bottomBarRect.size = UIVec(Camera::getViewport().size.x, totalHeight);
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, bottomBarRect.pos, bottomBarRect.pos + bottomBarRect.size);

    const float gapFromCenter = 120.;
    UIRect leftHandRect;
    leftHandRect.pos = bottomBarRect * UIVec(0., 0.) + UIVec(0., 0.);
    leftHandRect.size = bottomBarRect.size * UIVec(.5, 1.) + UIVec(-gapFromCenter, -padding);
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, leftHandRect.pos, leftHandRect.pos + leftHandRect.size);

    const float itemWindowSize = 80.;
    UIRect leftHandItemWindowRect;
    leftHandItemWindowRect.pos = leftHandRect * UIVec(1., 0.) + UIVec(-padding, padding) + UIVec(-itemWindowSize, 0.);
    leftHandItemWindowRect.size = UIVec(itemWindowSize, itemWindowSize);
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, leftHandItemWindowRect.pos, leftHandItemWindowRect.pos + leftHandItemWindowRect.size);
    Graphics::fillRect(sf::Color(255, 255, 255, 120), leftHandItemWindowRect.pos, leftHandItemWindowRect.pos + leftHandItemWindowRect.size);

    UIRect leftHandInfoRect;
    leftHandInfoRect.pos = leftHandRect * UIVec(0., 0.) + UIVec(padding, padding);
    leftHandInfoRect.size = leftHandRect.size * UIVec(1., 1.) + UIVec(-itemWindowSize - 3 * padding, -2 * padding);
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, leftHandInfoRect.pos, leftHandInfoRect.pos + leftHandInfoRect.size);

    if (game->player->inventory[Player::InventorySlots::LEFT_HAND] != nullptr) {
        Graphics::drawText(splitId(game->player->inventory[Player::InventorySlots::LEFT_HAND]->id).second, sf::Color::White, 24,
                           leftHandInfoRect * UIVec(1., 0.) + UIVec(-textLRPadding, 24.), 1.);

        UIRect leftMouseActionRect;
        leftMouseActionRect.pos = leftHandInfoRect * UIVec(1., 0.) + UIVec(-250, padding);
        leftMouseActionRect.size = UIVec(80., 24);

        Graphics::insertQuad(Graphics::Quad(
                1.,
                leftMouseActionRect * UIVec(0., 0.) + UIVec(0., 4.), sf::Vector2f(33. + 24. * 1., 8192. - 24.),
                leftMouseActionRect * UIVec(0., 0.) + UIVec(0. + 24., 4.), sf::Vector2f(33. + 24. * 2., 8192. - 24.),
                leftMouseActionRect * UIVec(0., 0.) + UIVec(0. + 24., 4. + 24.), sf::Vector2f(33. + 24. * 2., 8192.),
                leftMouseActionRect * UIVec(0., 0.) + UIVec(0., 4. + 24.), sf::Vector2f(33. + 24. * 1., 8192.)
        ));


        Graphics::insertQuad(Graphics::Quad(
                1.,
                leftMouseActionRect * UIVec(0., 0.) + UIVec(0., 36.), sf::Vector2f(0., 8192. - 20.),
                leftMouseActionRect * UIVec(0., 0.) + UIVec(0. + 33., 36.), sf::Vector2f(33., 8192. - 20.),
                leftMouseActionRect * UIVec(0., 0.) + UIVec(0. + 33., 36. + 20.), sf::Vector2f(33., 8192.),
                leftMouseActionRect * UIVec(0., 0.) + UIVec(0., 36. + 20.), sf::Vector2f(0., 8192.)
        ));

        Graphics::drawText("drop item", sf::Color::White, 20,
                           leftMouseActionRect * UIVec(0., 0.) + UIVec(26. + textLRPadding, 20.), 0.);
    }
}
