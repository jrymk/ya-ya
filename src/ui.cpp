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
    const float textYOffsetRatio = .34 * displayScaling; // multiply with font size BEFORE scaling
    const float controlsWidth = 400. * displayScaling;

    /// CONTROLS
    {
        UIRect controlsRect(bottomBarRect * UIVec(.5, 0.) - UIVec(controlsWidth / 2, 0.), controlsWidth, bottomBarRect.size.y);
        Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, controlsRect.pos, controlsRect.pos + controlsRect.size);

        float rowHeight = controlsRect.size.y / 3.;
        float imgCenterFromEdge = padding + 12. * displayScaling;
        float textFromEdge = imgCenterFromEdge + 12. * displayScaling + textLRPadding;

        if (game->player->inventory[Player::InventorySlots::LEFT_HAND] != nullptr) {
            Graphics::drawImage(imageMouseBase, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                sf::Color(255, 255, 255, 255));
            Graphics::drawImage(imageMouseLeftBtn, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                sf::Color(255, 170, 174, 255));
            Graphics::drawImage(imageMouseRightBtn, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                sf::Color(255, 255, 255, 255));
            Graphics::drawText(strDrop[1][0] + game->player->inventory[Player::InventorySlots::LEFT_HAND]->getLocalization(1, LOC_ENTITIY_NAME),
                               sf::Color(255, 255, 255, 255),
                               18 * displayScaling,
                               controlsRect * UIVec(0., 0.) + UIVec(textFromEdge, rowHeight * .5 + 18 * textYOffsetRatio), 0.);

        } else if (game->controls.facingEntity != nullptr) {
            Graphics::drawImage(imageMouseBase, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                sf::Color(255, 255, 255, 255));
            Graphics::drawImage(imageMouseLeftBtn, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                sf::Color(255, 170, 174, 255));
            Graphics::drawImage(imageMouseRightBtn, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                sf::Color(255, 255, 255, 255));
            Graphics::drawText(strHoldIn[1][0] + game->controls.facingEntity->getLocalization(1, LOC_ENTITIY_NAME) + strHoldIn[1][1] + strLeftHand[1],
                               sf::Color(255, 255, 255, 255),
                               18 * displayScaling,
                               controlsRect * UIVec(0., 0.) + UIVec(textFromEdge, rowHeight * .5 + 18 * textYOffsetRatio), 0.);
        }
    }

}
