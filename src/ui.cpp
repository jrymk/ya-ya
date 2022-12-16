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

        for (int hand = Player::InventorySlots::LEFT_HAND; hand <= Player::InventorySlots::RIGHT_HAND; hand++) {
            Controls::ControlsActions clickAction = (hand == 0 ? game->controls.leftMouseClickAction : game->controls.rightMouseClickAction);
            Controls::ControlsActions altClickAction = (hand == 0 ? game->controls.leftMouseAltClickAction : game->controls.rightMouseAltClickAction);

            for (int isAltAction = 0; isAltAction <= 1; isAltAction++) {
                Controls::ControlsActions action = (isAltAction ? altClickAction : clickAction);
                std::wstring actionStr = L"";

                switch (action) {
                    case Controls::PICK_UP_ITEM:
                    case Controls::PICK_UP_CONTAINER:
                        actionStr = strHoldIn[1][0] + game->controls.facingEntity->getLocalization(1, LOC_ENTITIY_NAME) + strHoldIn[1][1] + strLeftHand[1];
                        break;
                    case Controls::DROP_ITEM:
                    case Controls::DROP_CONTAINER:
                        actionStr = strDrop[1][0] + game->player->inventory[hand]->getLocalization(1, LOC_ENTITIY_NAME);
                        break;
                    case Controls::STORE_ITEM_TO_CONTAINER:
                        actionStr = L"儲存物品";
                        break;
                    case Controls::PICK_UP_ITEM_FROM_CONTAINER:
                        actionStr = L"從容器中拿出物品";
                        break;
                }

                if (actionStr == L"")
                    continue;

                if (hand == Player::InventorySlots::LEFT_HAND) {
                    if (!isAltAction) {
                        Graphics::drawImage(imageMouseBase, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 255, 255, 255));
                        Graphics::drawImage(imageMouseLeftBtn, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 170, 174, 255));
                        Graphics::drawImage(imageMouseRightBtn, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 255, 255, 255));
                        Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18 * displayScaling,
                                           controlsRect * UIVec(0., 0.) + UIVec(textFromEdge, rowHeight * .5 + 18 * textYOffsetRatio), 0.);
                    }
                    if (isAltAction) {
                        Graphics::drawImage(imageAltKey, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * 1.5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 255, 255, 255));
                        Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18 * displayScaling,
                                           controlsRect * UIVec(0., 0.) + UIVec(textFromEdge, rowHeight * 1.5 + 18 * textYOffsetRatio), 0.);
                    }
                }
                if (hand == Player::InventorySlots::RIGHT_HAND) {
                    if (!isAltAction) {
                        Graphics::drawImage(imageMouseBase, 1., controlsRect * UIVec(1., 0.) + UIVec(-imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 255, 255, 255));
                        Graphics::drawImage(imageMouseLeftBtn, 1., controlsRect * UIVec(1., 0.) + UIVec(-imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 255, 255, 255));
                        Graphics::drawImage(imageMouseRightBtn, 1., controlsRect * UIVec(1., 0.) + UIVec(-imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 170, 174, 255));
                        Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18 * displayScaling,
                                           controlsRect * UIVec(1., 0.) + UIVec(-textFromEdge, rowHeight * .5 + 18 * textYOffsetRatio), 1.);
                    }
                    if (isAltAction) {
                        Graphics::drawImage(imageAltKey, 1., controlsRect * UIVec(1., 0.) + UIVec(-imgCenterFromEdge, rowHeight * 1.5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 255, 255, 255));
                        Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18 * displayScaling,
                                           controlsRect * UIVec(1., 0.) + UIVec(-textFromEdge, rowHeight * 1.5 + 18 * textYOffsetRatio), 1.);
                    }
                }
            }


//            if (game->player->inventory[Player::InventorySlots::LEFT_HAND] != nullptr) {
//
//            }
//            else if (game->controls.facingEntity != nullptr) {
//                Graphics::drawImage(imageMouseBase, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
//                                    sf::Color(255, 255, 255, 255));
//                Graphics::drawImage(imageMouseLeftBtn, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
//                                    sf::Color(255, 170, 174, 255));
//                Graphics::drawImage(imageMouseRightBtn, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * .5), UIVec(0.5, 0.5), displayScaling,
//                                    sf::Color(255, 255, 255, 255));
//                Graphics::drawText(strHoldIn[1][0] + game->controls.facingEntity->getLocalization(1, LOC_ENTITIY_NAME) + strHoldIn[1][1] + strLeftHand[1],
//                                   sf::Color(255, 255, 255, 255),
//                                   18 * displayScaling,
//                                   controlsRect * UIVec(0., 0.) + UIVec(textFromEdge, rowHeight * .5 + 18 * textYOffsetRatio), 0.);
//            }
        }
    }

}
