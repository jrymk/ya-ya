#include "ui.h"
#include "game.h"
#include "graphics.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "localization.h"
#include "model.h"
#include "gamecontroller.h"

void UserInterface::renderUI() {
    Graphics::setFont(0);
    const float totalHeight = 90. * displayScaling;
    const float bottomPadding = 8. * displayScaling;
    const float padding = 12. * displayScaling;
    const float textLRPadding = 4. * displayScaling;
    UIRect bottomBarRect(Camera::getViewport() * UIVec(0., 1.) + UIVec(0., -totalHeight - bottomPadding), Camera::getViewport().size.x, totalHeight);
    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, bottomBarRect.pos, bottomBarRect.pos + bottomBarRect.size);
    const float textYOffsetRatio = .34 * displayScaling; // multiply with font size BEFORE scaling
    const float controlsWidth = 400. * displayScaling;

    /// CONTROLS
    UIRect controlsRect(bottomBarRect * UIVec(.5, 0.) - UIVec(controlsWidth / 2, 0.), controlsWidth, bottomBarRect.size.y);
    {
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
                    case Controls::PICK_UP_ITEM_FROM_FACING_CONTAINER:
                        actionStr = L"從容器中拿出物品";
                        break;
                    case Controls::DROP_ITEM:
                    case Controls::DROP_CONTAINER:
                        actionStr = strDrop[1][0] + game->player->inventory[hand]->getLocalization(1, LOC_ENTITIY_NAME);
                        break;
                    case Controls::STORE_ITEM_TO_FACING_CONTAINER:
                        actionStr = L"儲存物品";
                        break;
                    case Controls::STORE_ITEM_TO_OTHER_HAND_CONTAINER:
                        actionStr = L"STORE_ITEM_TO_OTHER_HAND_CONTAINER";
                        break;
                    case Controls::STORE_FACING_ITEM_TO_CONTAINER:
                        actionStr = L"STORE_FACING_ITEM_TO_CONTAINER";
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
                        Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18. * displayScaling,
                                           controlsRect * UIVec(0., 0.) + UIVec(textFromEdge, rowHeight * .5 + 18. * textYOffsetRatio), 0.);
                    }
                    if (isAltAction) {
                        Graphics::drawImage(imageAltKey, 1., controlsRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * 1.5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 255, 255, 255));
                        Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18. * displayScaling,
                                           controlsRect * UIVec(0., 0.) + UIVec(textFromEdge, rowHeight * 1.5 + 18. * textYOffsetRatio), 0.);
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
                        Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18. * displayScaling,
                                           controlsRect * UIVec(1., 0.) + UIVec(-textFromEdge, rowHeight * .5 + 18. * textYOffsetRatio), 1.);
                    }
                    if (isAltAction) {
                        Graphics::drawImage(imageAltKey, 1., controlsRect * UIVec(1., 0.) + UIVec(-imgCenterFromEdge, rowHeight * 1.5), UIVec(0.5, 0.5), displayScaling,
                                            sf::Color(255, 255, 255, 255));
                        Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18. * displayScaling,
                                           controlsRect * UIVec(1., 0.) + UIVec(-textFromEdge, rowHeight * 1.5 + 18. * textYOffsetRatio), 1.);
                    }
                }
            }
        }
    } /// CONTROLS

    /// INFO
    UIRect infoRect(bottomBarRect * UIVec(0., 0.), controlsRect.pos.x - bottomBarRect.pos.x - padding, bottomBarRect.size.y);
    {
        Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, infoRect.pos, infoRect.pos + infoRect.size);

        float avatarRectSize = 64. * displayScaling;
        UIRect avatarRect(infoRect * UIVec(1., 0.) + UIVec(-avatarRectSize, 0.), avatarRectSize, avatarRectSize);
        Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, avatarRect.pos, avatarRect.pos + avatarRect.size);

        float nameRectWidth = 300. * displayScaling;
        UIRect nameRect(avatarRect.pos.x - padding - nameRectWidth, infoRect.pos.y, nameRectWidth, infoRect.size.y);
        Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, nameRect.pos, nameRect.pos + nameRect.size);

        if (game->player->inventory[Player::InventorySlots::LEFT_HAND] != nullptr)
            Graphics::drawText(game->player->inventory[Player::InventorySlots::LEFT_HAND]->id, sf::Color(255, 255, 255, 255), 22. * displayScaling,
                               nameRect * UIVec(1., 0.) + UIVec(-textLRPadding, 14. * displayScaling + 22. * textYOffsetRatio), 1.);
    } /// INFO
}

void UserInterface::renderOverlay() {
    /// OVERLAY
    Graphics::drawText(L"$" + toWStr(game->controller.cash),
                       sf::Color(255, 255, 255, 255), displayScaling * 32,
                       Camera::getViewport() * UIVec(1., 0.) + UIVec(-12, displayScaling * 32 * 0.9), 1.);
    {
        switch (game->controller.gameState) {
            case GameController::TITLE_SCREEN:
                Graphics::setFont(0);
                Graphics::fillRect(sf::Color(62, 88, 122, 255), Camera::getViewport().pos, Camera::getViewport().pos + Camera::getViewport().size);
                Graphics::drawText(L"烤鴨鴨", sf::Color(255, 255, 255, 255), displayScaling * 150, Camera::getViewport() * UIVec(.5, .5), .5);
                Graphics::drawText(L"按F10啦", sf::Color(255, 255, 255, 255), displayScaling * 32, Camera::getViewport() * UIVec(.5, .7), .5);
                Graphics::drawText(L"弱弱的說: F9 to load", sf::Color(255, 255, 255, 255), displayScaling * 20, Camera::getViewport() * UIVec(.5, .9), .5);

                break;

            case GameController::DAY_START_SCENE: {
                double time = -game->controller.startOfDayTp.elapsed();
                int opacity = time < .5 ? (time / .5 * 255) : 255;
                Graphics::setFont(0);
                Graphics::fillRect(sf::Color(62, 88, 122, opacity), Camera::getViewport().pos, Camera::getViewport().pos + Camera::getViewport().size);
                Graphics::drawText(L"第" + toWStr(game->controller.dayCount) + L"天", sf::Color(255, 255, 255, opacity), displayScaling * 120,
                                   Camera::getViewport() * UIVec(.5, .5), .5);
                /// TODO: change animation
                break;
            }

        }
    } /// OVERLAY

}