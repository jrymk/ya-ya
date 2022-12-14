#include "ui.h"
#include "game.h"
#include "graphics.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "truck.h"
#include "localization.h"
#include "model.h"
#include "gamecontroller.h"

void UserInterface::renderUI() {
    if (game->controller.gameState == GameController::TITLE_SCREEN) return;  // no render on title
    Graphics::setFont(0);
    const float totalHeight = 140. * displayScaling;
    const float bottomPadding = 8. * displayScaling;
    const float padding = 12. * displayScaling;
    const float textLRPadding = 4. * displayScaling;
    UIRect bottomBarRect(Camera::getViewport() * UIVec(0., 1.) + UIVec(0., -totalHeight - bottomPadding), Camera::getViewport().size.x, totalHeight);
//    Graphics::drawRect(sf::Color(0, 0, 0, 40), 1, bottomBarRect.pos, bottomBarRect.pos + bottomBarRect.size);
    const float textYOffsetRatio = .34 * displayScaling; // multiply with font size BEFORE scaling
    const float controlsWidth = 400. * displayScaling;

    /// CONTROLS
    float rowHeight = bottomBarRect.size.y / 5.;
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
                    actionStr = L"????????????????????????";
                    break;
                case Controls::DROP_ITEM:
                case Controls::DROP_CONTAINER:
                    actionStr = strDrop[1][0] + game->player->inventory[hand]->getLocalization(1, LOC_ENTITIY_NAME);
                    break;
                case Controls::STORE_ITEM_TO_FACING_CONTAINER:
                    actionStr = L"????????????";
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
                    Graphics::drawImage(imageMouseBase, 1., bottomBarRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * 3.5), UIVec(0.5, 0.5), displayScaling,
                                        sf::Color(255, 255, 255, 255));
                    Graphics::drawImage(imageMouseLeftBtn, 1., bottomBarRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * 3.5), UIVec(0.5, 0.5), displayScaling,
                                        sf::Color(255, 170, 174, 255));
                    Graphics::drawImage(imageMouseRightBtn, 1., bottomBarRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * 3.5), UIVec(0.5, 0.5), displayScaling,
                                        sf::Color(255, 255, 255, 255));
                    Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18. * displayScaling,
                                       bottomBarRect * UIVec(0., 0.) + UIVec(textFromEdge, rowHeight * 3.5 + 18. * textYOffsetRatio), 0.);
                }
                if (isAltAction) {
                    Graphics::drawImage(imageAltKey, 1., bottomBarRect * UIVec(0., 0.) + UIVec(imgCenterFromEdge, rowHeight * 4.5), UIVec(0.5, 0.5), displayScaling,
                                        sf::Color(255, 255, 255, 255));
                    Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18. * displayScaling,
                                       bottomBarRect * UIVec(0., 0.) + UIVec(textFromEdge, rowHeight * 4.5 + 18. * textYOffsetRatio), 0.);
                }
            }
            if (hand == Player::InventorySlots::RIGHT_HAND) {
                if (!isAltAction) {
                    Graphics::drawImage(imageMouseBase, 1., bottomBarRect * UIVec(1., 0.) + UIVec(-imgCenterFromEdge, rowHeight * 3.5), UIVec(0.5, 0.5), displayScaling,
                                        sf::Color(255, 255, 255, 255));
                    Graphics::drawImage(imageMouseLeftBtn, 1., bottomBarRect * UIVec(1., 0.) + UIVec(-imgCenterFromEdge, rowHeight * 3.5), UIVec(0.5, 0.5), displayScaling,
                                        sf::Color(255, 255, 255, 255));
                    Graphics::drawImage(imageMouseRightBtn, 1., bottomBarRect * UIVec(1., 0.) + UIVec(-imgCenterFromEdge, rowHeight * 3.5), UIVec(0.5, 0.5), displayScaling,
                                        sf::Color(255, 170, 174, 255));
                    Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18. * displayScaling,
                                       bottomBarRect * UIVec(1., 0.) + UIVec(-textFromEdge, rowHeight * 3.5 + 18. * textYOffsetRatio), 1.);
                }
                if (isAltAction) {
                    Graphics::drawImage(imageAltKey, 1., bottomBarRect * UIVec(1., 0.) + UIVec(-imgCenterFromEdge, rowHeight * 4.5), UIVec(0.5, 0.5), displayScaling,
                                        sf::Color(255, 255, 255, 255));
                    Graphics::drawText(actionStr, sf::Color(255, 255, 255, 255), 18. * displayScaling,
                                       bottomBarRect * UIVec(1., 0.) + UIVec(-textFromEdge, rowHeight * 4.5 + 18. * textYOffsetRatio), 1.);
                }
            }
        }

        /// INFO
        if (game->player->inventory[Player::InventorySlots::LEFT_HAND] != nullptr) {
            Graphics::drawText(game->player->inventory[Player::InventorySlots::LEFT_HAND]->getLocalization(1, LOC_ENTITIY_NAME), sf::Color(255, 255, 255, 255),
                               24. * displayScaling,
                               bottomBarRect * UIVec(0., 0.) + UIVec(12. * displayScaling, rowHeight * 0.95 + 24. * textYOffsetRatio), 0.);
            Graphics::drawText(game->player->inventory[Player::InventorySlots::LEFT_HAND]->getLocalization(1, LOC_ENTITIY_DESC), sf::Color(255, 255, 255, 255),
                               18. * displayScaling,
                               bottomBarRect * UIVec(0., 0.) + UIVec(12. * displayScaling, rowHeight * 2.0 + 18. * textYOffsetRatio), 0.);
        }
        if (game->player->inventory[Player::InventorySlots::RIGHT_HAND] != nullptr) {
            Graphics::drawText(game->player->inventory[Player::InventorySlots::RIGHT_HAND]->getLocalization(1, LOC_ENTITIY_NAME), sf::Color(255, 255, 255, 255),
                               24. * displayScaling,
                               bottomBarRect * UIVec(1., 0.) + UIVec(-12. * displayScaling, rowHeight * 0.95 + 24. * textYOffsetRatio), 1.);
            Graphics::drawText(game->player->inventory[Player::InventorySlots::RIGHT_HAND]->getLocalization(1, LOC_ENTITIY_DESC), sf::Color(255, 255, 255, 255),
                               18. * displayScaling,
                               bottomBarRect * UIVec(1., 0.) + UIVec(-12. * displayScaling, rowHeight * 2.0 + 18. * textYOffsetRatio), 1.);
        }

        /// INVENTORY (SHOP)
        game->controls.onHoverButton = Controls::UIButtons::NONE;
        int itemCount = 0;
        if (game->controller.cash >= FENCE_COST)
            itemCount++;
        if (game->controller.cash >= MOAI_COST)
            itemCount++;

        for (int i = 0; i < itemCount; i++) {
            UIVec posOffset(72. * displayScaling * (-float(itemCount - 1) / 2. + i), rowHeight * 3.7);

            UIRect buttonRect = Graphics::drawImageGetRect(imageItemButtonBackground, 1.,
                                                           bottomBarRect * UIVec(.5, 0.) + posOffset, UIVec(.5, .5), 1. * displayScaling,
                                                           sf::Color(255, 255, 255, 40));
            bool buttonHover = false;
            if ((std::abs(((Camera::getMousePos() - buttonRect.pos).x / buttonRect.size.x) - 0.5)) <= 0.5 &&
                (std::abs(((Camera::getMousePos() - buttonRect.pos).y / buttonRect.size.y) - 0.5)) <= 0.5) {
                buttonHover = true;
            }
            if (i == 0) {
                if (buttonHover)
                    game->controls.onHoverButton = Controls::UIButtons::BUILD_FENCE;
                Graphics::drawImage(imageItemButtonFence, 1. + ZDEPTH_LAYER, bottomBarRect * UIVec(.5, 0.) + posOffset, UIVec(.5, .5),
                                    (buttonHover ? 1.15 : 1.) * displayScaling, sf::Color(255, 255, 255, 255));
            }
            if (i == 1) {
                if (buttonHover)
                    game->controls.onHoverButton = Controls::UIButtons::BUILD_MOAI; // I don't care
                Graphics::drawImage(imageItemButtonMoai, 1. + ZDEPTH_LAYER, bottomBarRect * UIVec(.5, 0.) + posOffset, UIVec(.5, .5),
                                    (buttonHover ? 1.15 : 1.) * displayScaling, sf::Color(255, 255, 255, 255));
            }
        }

        if (game->controller.cash >= MOAI_COST) {

        }
    }
}

void UserInterface::renderOverlay() {
    const float textYOffsetRatio = .34 * displayScaling; // multiply with font size BEFORE scaling

    /// OVERLAY
    Graphics::setFont(1);
    Graphics::drawText(L"$" + toWStr(game->controller.cash),
                       sf::Color(255, 255, 255, 255), displayScaling * 32,
                       Camera::getViewport() * UIVec(0., 0.) + UIVec(16. * displayScaling, 24. * displayScaling + 32. * textYOffsetRatio), 0.);
    Graphics::setFont(0);
    UIVec timeOfDayBarSize(240. * displayScaling, 8. * displayScaling);
    Graphics::fillRect(sf::Color(255, 255, 255, 200),
                       Camera::getViewport() * UIVec(1., 0.) - timeOfDayBarSize + UIVec(-24, 20) * displayScaling,
                       Camera::getViewport() * UIVec(1., 0.) + UIVec(-24, 20) * displayScaling);
    Graphics::fillRect(sf::Color(255, 187, 0, 255),
                       Camera::getViewport() * UIVec(1., 0.) - timeOfDayBarSize + UIVec(-24, 20) * displayScaling,
                       Camera::getViewport() * UIVec(1., 0.) - timeOfDayBarSize + UIVec(-24, 20) * displayScaling +
                       UIVec(timeOfDayBarSize.x * game->controller.startOfDayTp.elapsed() / DAY_LENGTH, timeOfDayBarSize.y));
    Graphics::drawText(L"???" + toWStr(game->controller.dayCount) + L"???", sf::Color::White, 20. * displayScaling,
                       Camera::getViewport() * UIVec(1., 0.) + UIVec(-24 * displayScaling, 40. * displayScaling + 20. * textYOffsetRatio), 1.);

    switch (game->controller.gameState) {
        case GameController::TITLE_SCREEN: {
            game->controls.onHoverButton = Controls::NONE;
            Graphics::setFont(0);
            Graphics::fillRect(sf::Color(49, 94, 61, 255), Camera::getViewport().pos, Camera::getViewport().pos + Camera::getViewport().size);
            Graphics::drawText(L"?????????", sf::Color(255, 255, 255, 255), displayScaling * 150, Camera::getViewport() * UIVec(.5, .5), .5);
            UIVec buttonSize(480. * displayScaling, 64. * displayScaling);
            UIRect buttonRect(Camera::getViewport() * UIVec(.5, .8) - buttonSize * .5, buttonSize);
            UIRect buttonRect2(Camera::getViewport() * UIVec(.5, .8) - buttonSize * .5 + UIVec(0., buttonSize.y + 16. * displayScaling), buttonSize);
            if ((std::abs(((Camera::getMousePos() - buttonRect.pos).x / buttonRect.size.x) - 0.5)) <= 0.5 &&
                (std::abs(((Camera::getMousePos() - buttonRect.pos).y / buttonRect.size.y) - 0.5)) <= 0.5) {
                game->controls.onHoverButton = Controls::LOAD_GAME;
            }
            if ((std::abs(((Camera::getMousePos() - buttonRect2.pos).x / buttonRect2.size.x) - 0.5)) <= 0.5 &&
                (std::abs(((Camera::getMousePos() - buttonRect2.pos).y / buttonRect2.size.y) - 0.5)) <= 0.5) {
                game->controls.onHoverButton = Controls::START_NEW_GAME;
            }

            Graphics::fillRect(sf::Color(255, 255, 255, 100), buttonRect * UIVec(0., 0.), buttonRect * UIVec(1., 1.));
            Graphics::drawText(L"??????????????????", sf::Color(255, 255, 255, 255), displayScaling * 32 * (game->controls.onHoverButton == Controls::LOAD_GAME ? 1.1 : 1.),
                               buttonRect * UIVec(.5, .5) + UIVec(0., textYOffsetRatio * 32.), .5);
            Graphics::fillRect(sf::Color(255, 255, 255, 100), buttonRect2 * UIVec(0., 0.), buttonRect2 * UIVec(1., 1.));
            Graphics::drawText(L"?????????????????????", sf::Color(255, 255, 255, 255), displayScaling * 32 * (game->controls.onHoverButton == Controls::START_NEW_GAME ? 1.1 : 1.),
                               buttonRect2 * UIVec(.5, .5) + UIVec(0., textYOffsetRatio * 32.), .5);

            Graphics::drawImage(imageCredit, 1.4, Camera::getViewport() * UIVec(1., 1.) + UIVec(-16., -16.) * displayScaling, UIVec(1., 1.), displayScaling,
                                sf::Color(255, 255, 255, 150));
            break;
        }
        case GameController::DAY_END_SCENE: {
            double time = game->controller.startOfDayTp.elapsed() - DAY_LENGTH;
            int opacity = time < .5 ? (time / .5 * 255) : 255;
            Graphics::setFont(0);
            Graphics::fillRect(sf::Color(49, 94, 61, opacity), Camera::getViewport().pos, Camera::getViewport().pos + Camera::getViewport().size);
            Graphics::drawText(L"???????????????$ " + toWStr(game->controller.goodsAvailableForSale) + L" !", sf::Color(200, 225, 250, opacity), displayScaling * 60,
                               Camera::getViewport() * UIVec(.5, .25), .5);
            Graphics::drawText(L"?????????", sf::Color(255, 255, 255, 255), displayScaling * 36, Camera::getViewport() * UIVec(.5, .4), .4);
            Graphics::drawText(toWStr(game->controller.goodsSold[GameController::G_GROWN]) + L" ?????????", sf::Color(255, 255, 255, 255), displayScaling * 32,
                                Camera::getViewport() * UIVec(.5, .5), .5);
            Graphics::drawText(toWStr(game->controller.goodsSold[GameController::G_CHILD]) + L" ?????????", sf::Color(255, 255, 255, 255), displayScaling * 32,
                                Camera::getViewport() * UIVec(.5, .575), .5);
            Graphics::drawText(toWStr(game->controller.goodsSold[GameController::G_DUCKLING]) + L" ?????????", sf::Color(255, 255, 255, 255), displayScaling * 32,
                                Camera::getViewport() * UIVec(.5, .65), .5);
            Graphics::drawText(toWStr(game->controller.goodsSold[GameController::G_UNFERTILIZED_EGG]) + L" ?????????", sf::Color(255, 255, 255, 255), displayScaling * 32,
                                Camera::getViewport() * UIVec(.5, .725), .5);
            if(game->controller.goodsSold[GameController::G_FERTILIZED_EGG])
                Graphics::drawText(L"?????? " + toWStr(game->controller.goodsSold[GameController::G_FERTILIZED_EGG]) + L" ??????????????????????????? :)", sf::Color(250, 185, 185, 255),
                                    displayScaling * 32, Camera::getViewport() * UIVec(.5, .8), .5);
            break;
        }
        case GameController::DAY_START_SCENE: {
            double time = -game->controller.startOfDayTp.elapsed();
            int opacity = time < .5 ? std::max(.0, (time / .5 * 255)) : 255;
            Graphics::setFont(0);
            Graphics::fillRect(sf::Color(49, 94, 61, opacity), Camera::getViewport().pos, Camera::getViewport().pos + Camera::getViewport().size);
            Graphics::drawText(L"???" + toWStr(game->controller.dayCount) + L"???", sf::Color(255, 255, 255, opacity), displayScaling * 120,
                               Camera::getViewport() * UIVec(.5, .5), .5);
            /// TODO: change animation
            break;
        }


    }

    if (game->controller.gameState == GameController::TITLE_SCREEN) return;  // no truck pos on title
    UIVec pos = Camera::getViewport() * UIVec(.5, .5) + (Camera::getScreenPos(game->truck->position) - Camera::getViewport() * UIVec(.5, .5)) * 1.;
    pos = pos.max(Camera::getViewport() * UIVec(0., 0.) + UIVec(36, 36) * displayScaling);
    pos = pos.min(Camera::getViewport() * UIVec(1., 1.) - UIVec(36, 36) * displayScaling);

    float scale = 1.;
    scale *= imageTruckIcon.defaultScale * displayScaling;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F8) && (pos.x == (Camera::getViewport() * UIVec(0., 0.) + UIVec(36, 36) * displayScaling).x ||
                                                         pos.x == (Camera::getViewport() * UIVec(1., 1.) - UIVec(36, 36) * displayScaling).x ||
                                                         pos.y == (Camera::getViewport() * UIVec(0., 0.) + UIVec(36, 36) * displayScaling).y ||
                                                         pos.y == (Camera::getViewport() * UIVec(1., 1.) - UIVec(36, 36) * displayScaling).y)) {
        Graphics::insertQuad(Graphics::Quad(
                1.1,
                pos + UIVec(imageTruckIcon.textureSize) * UIVec(0., 0.) - (UIVec(imageTruckIcon.textureSize) * scale) * UIVec(.5, .5),
                imageTruckIcon.texturePos,
                pos + UIVec(imageTruckIcon.textureSize) * UIVec(scale, 0.) - (UIVec(imageTruckIcon.textureSize) * scale) * UIVec(.5, .5),
                imageTruckIcon.texturePos + sf::Vector2f(imageTruckIcon.textureSize.x, 0.),
                pos + UIVec(imageTruckIcon.textureSize) * UIVec(scale, scale) - (UIVec(imageTruckIcon.textureSize) * scale) * UIVec(.5, .5),
                imageTruckIcon.texturePos + imageTruckIcon.textureSize,
                pos + UIVec(imageTruckIcon.textureSize) * UIVec(0., scale) - (UIVec(imageTruckIcon.textureSize) * scale) * UIVec(.5, .5),
                imageTruckIcon.texturePos + sf::Vector2f(0., imageTruckIcon.textureSize.y),
                sf::Color(0, 140, 255, 150)
        ));
    }
}

bool UserInterface::drawButton(const std::wstring &str, UIRect buttonRect, sf::Color bgColor, sf::Color textColor) {

    return false;
}
