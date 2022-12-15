#include "ui.h"
#include "game.h"
#include "graphics.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "localization.h"
#include "model.h"

void UserInterface::renderUI() {
    Graphics::setFont(4);
    const float totalHeight = 90. * displayScaling;
    const float bottomPadding = 8. * displayScaling;
    const float padding = 12. * displayScaling;
    const float textLRPadding = 4. * displayScaling;

    UIRect bottomBarRect(Camera::getViewport() * UIVec(0., 1.) + UIVec(0., -totalHeight - bottomPadding), Camera::getViewport().size.x, totalHeight);
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, bottomBarRect.pos, bottomBarRect.pos + bottomBarRect.size);

    const float controlsWidth = 400. * displayScaling;

    UIRect controlsRect(bottomBarRect * UIVec(.5, 0.) - UIVec(controlsWidth / 2, 0.), controlsWidth, bottomBarRect.size.y);
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, controlsRect.pos, controlsRect.pos + controlsRect.size);

    UIRect controlsRow1(controlsRect * UIVec(0., 0.), controlsRect.size * UIVec(1., 1. / 3.));
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, controlsRow1.pos, controlsRow1.pos + controlsRow1.size);

    UIRect controlsRow2(controlsRect * UIVec(0., 1. / 3.), controlsRect.size * UIVec(1., 1. / 3.));
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, controlsRow2.pos, controlsRow2.pos + controlsRow2.size);

    UIRect controlsRow3(controlsRect * UIVec(0., 2. / 3.), controlsRect.size * UIVec(1., 1. / 3.));
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, controlsRow3.pos, controlsRow3.pos + controlsRow3.size);

    const float controlsImageCenterMargin = 22. * displayScaling;
    UIRect controlsColImageLeft(controlsRect * UIVec(0., 0.) + UIVec(controlsImageCenterMargin, 0.), controlsImageCenterMargin + textLRPadding, controlsRect.size.y);
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, controlsColImageLeft.pos, controlsColImageLeft.pos + controlsColImageLeft.size);
    UIRect controlsColImageRight(controlsRect * UIVec(1., 0.) + UIVec(-controlsImageCenterMargin - (controlsImageCenterMargin + textLRPadding), 0.),
                                 controlsImageCenterMargin + padding + textLRPadding, controlsRect.size.y);
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, controlsColImageRight.pos, controlsColImageRight.pos + controlsColImageRight.size);


//    UIRect leftHandRect;
//    leftHandRect.pos = bottomBarRect * UIVec(0., 0.) + UIVec(0., 0.);
//    leftHandRect.size = bottomBarRect.size * UIVec(.5, 1.) + UIVec(-gapFromCenter, -padding);
//    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, leftHandRect.pos, leftHandRect.pos + leftHandRect.size);
//
//    UIRect rightHandRect;
//    rightHandRect.pos = bottomBarRect * UIVec(.5, 0.) + UIVec(gapFromCenter, 0.);
//    rightHandRect.size = bottomBarRect.size * UIVec(.5, 1.) + UIVec(-gapFromCenter, -padding);
//    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, rightHandRect.pos, rightHandRect.pos + rightHandRect.size);
//
//    const float itemWindowSize = 80.;
//
//    UIRect leftHandItemWindowRect;
//    leftHandItemWindowRect.pos = leftHandRect * UIVec(1., 0.) + UIVec(-padding, padding) + UIVec(-itemWindowSize, 0.);
//    leftHandItemWindowRect.size = UIVec(itemWindowSize, itemWindowSize);
//    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, leftHandItemWindowRect.pos, leftHandItemWindowRect.pos + leftHandItemWindowRect.size);
//    Graphics::fillRect(sf::Color(255, 255, 255, 120), leftHandItemWindowRect.pos, leftHandItemWindowRect.pos + leftHandItemWindowRect.size);
//
//    UIRect leftHandInfoRect;
//    leftHandInfoRect.pos = leftHandRect * UIVec(0., 0.) + UIVec(padding, padding);
//    leftHandInfoRect.size = leftHandRect.size * UIVec(1., 1.) + UIVec(-itemWindowSize - 3 * padding, -2 * padding);
//    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, leftHandInfoRect.pos, leftHandInfoRect.pos + leftHandInfoRect.size);
//
//    UIRect controlsRect;
//    controlsRect.pos = leftHandRect * UIVec(1., 0.) + UIVec(padding, padding);
//    controlsRect.size = UIVec(gapFromCenter * 2 - 2 * padding, leftHandRect.size.y - 2 * padding);
//    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, controlsRect.pos, controlsRect.pos + controlsRect.size);
//
    if (game->player->inventory[Player::InventorySlots::LEFT_HAND] != nullptr) {
//        Graphics::drawText(splitId(game->player->inventory[Player::InventorySlots::LEFT_HAND]->id).second, sf::Color::White, 24,
//                           leftHandInfoRect * UIVec(1., 0.) + UIVec(-textLRPadding, 24.), 1.);

//        UIRect leftMouseActionRect;
//        leftMouseActionRect.pos = controlsRow1 * UIVec(0., 0.) + UIVec(-250, padding);
//        leftMouseActionRect.size = UIVec(80., 24);

        Graphics::drawImage(imageMouseLeftBtn, 1., UIVec(controlsColImageLeft.pos.x, controlsRow1.pos.y + controlsRow1.size.y * 0.5), UIVec(0.5, 0.5), displayScaling);
        Graphics::drawImage(imageAltKey, 1., UIVec(controlsColImageLeft.pos.x, controlsRow2.pos.y + controlsRow2.size.y * 0.5), UIVec(0.5, 0.5), displayScaling);

        Graphics::drawText(strDrop[1][0] + game->player->inventory[Player::InventorySlots::LEFT_HAND]->getLocalization(1, LOC_ENTITIY_NAME),
                           sf::Color::White, 18 * displayScaling,
                           UIVec(controlsColImageLeft.pos.x + controlsColImageLeft.size.x, controlsRow1.pos.y + controlsRow1.size.y * 0.5 + 6. * displayScaling), 0.);
        Graphics::drawText(strDrop[1][0] + game->player->inventory[Player::InventorySlots::LEFT_HAND]->getLocalization(1, LOC_ENTITIY_NAME),
                           sf::Color::White, 18 * displayScaling,
                           UIVec(controlsColImageLeft.pos.x + controlsColImageLeft.size.x, controlsRow2.pos.y + controlsRow2.size.y * 0.5 + 6. * displayScaling), 0.);
    } else if (game->controls.facingEntity != nullptr) {
        Graphics::drawImage(imageMouseLeftBtn, 1., UIVec(controlsColImageLeft.pos.x, controlsRow1.pos.y + controlsRow1.size.y * 0.5), UIVec(0.5, 0.5), displayScaling);
        Graphics::drawText(strHoldIn[1][0] + game->controls.facingEntity->getLocalization(1, LOC_ENTITIY_NAME) + strHoldIn[1][1] + strLeftHand[1], sf::Color::White,
                           18 * displayScaling,
                           UIVec(controlsColImageLeft.pos.x + controlsColImageLeft.size.x, controlsRow1.pos.y + controlsRow1.size.y * 0.5 + 6. * displayScaling), 0.);
    }
}
