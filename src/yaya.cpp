#include "graphics.h"
#include "camera.h"
#include "utilities.h"
#include "game.h"
#include "player.h"
#include "duck.h"
#include "egg.h"
#include "ui.h"
#include "eggcarton.h"
#include "truck.h"
#include "audio.h"

//#define SUSPEND_CMD

/**
 * Key definitions
 * F1: toggle wireframe
 * F2: toggle collision boxes
 * F3: clear debug output
 * F4: toggle debug out of sight
 * F5: duckwalk to cursor
 * F6: toggle action list
 * F7: destroy all ducks
 * F8: show truck
 * F10: exit title screen
 * F11: toggle fullscreen
 */

int main() {
    // window, graphics and audio initialization
    sf::RenderWindow window;
    Camera::tieRenderWindow(window);
    Graphics::setRenderWindow(window);
    Graphics::createWindow(false);
    Graphics::loadFont(0, "./res/SourceHanSansTC-Bold.otf");
    Graphics::loadFont(1, "./res/PlusJakartaSans-Bold.ttf");
    sf::Texture tilemap;
    if (!tilemap.loadFromFile("./res/tilemap.png"))
        debug << "failed to load tilemap.png";
    tilemap.setSmooth(true);
    FramerateCounter fc;

    Audio audio;
    audio.loadSound("./res/walk.wav");
    audio.loadSound("./res/tick.wav");
    audio.loadSound("./res/yaya music.wav"); // always have to be the 3rd audio input unless you change BGM_ID

    /// game initialization
    Game game;

#ifndef TESTLOAD
    game.controller.loadTestWorld();
#else
    game.controller.loadToSaveFile();
#endif

    game.initTruckCollisionBoxes(3, -2);

    audio.playBGM();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    Graphics::resizeView(event.size.width, event.size.height);
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::F10) {
                        game.controller.loadTestWorld();  // TBD
                        game.initTruckCollisionBoxes(3, -2);
                    }
                    if (event.key.code == sf::Keyboard::F9) {
                        game.controller.loadToSaveFile();
                        game.initTruckCollisionBoxes(3, -2);
                        game.controller.handleAction(GameController::BTN_START_LOAD_GAME);
                    }
                    break;
                case sf::Event::KeyReleased:
                    game.controls.handleKeyPress(event);
                    game.controls.handleSoundOnAction(event, audio);
                    break;
                case sf::Event::MouseButtonPressed:
                case sf::Event::MouseButtonReleased:
                    game.controls.handleMousePress(event);
                    game.controls.handleSoundOnAction(event, audio);
                    break;
                case sf::Event::MouseWheelMoved:
                    game.controls.handleMouseScroll(event);
                    break;
            }

            window.clear(sf::Color(129, 214, 131));
            UIRect rectWindow(sf::FloatRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
            Camera::setViewport(rectWindow);

            /// render pass 1
            if (game.controller.gameState != GameController::TITLE_SCREEN)
                Camera::setCenter(Camera::getCenter() + (game.player->position - Camera::getCenter()) * 0.1); /// TODO: decide underlying pos or pos
            Graphics::clearQuadsArray();

            game.controller.update();

            game.update();

            game.controls.update();
            // this should be AFTER game.update() so when the next main loop comes, the controls event are polled and executed, the states are up to date
            // but BEFORE renderUI() so it gets the newest states

            game.render();
            Graphics::renderQuads(window, tilemap, Camera::getViewport());


            /// render pass 2
            Graphics::clearQuadsArray();
            game.ui.renderUI();
            Graphics::renderQuads(window, tilemap, Camera::getViewport());

            /// render pass 3 (overlay)
            Graphics::clearQuadsArray();
            game.ui.renderOverlay();
            Graphics::renderQuads(window, tilemap, Camera::getViewport());


            window.display();
        }
    }

    game.controller.saveToSaveFile();

#ifdef SUSPEND_CMD
    getchar();  // for testing with cmd
#endif
    return 0;
}