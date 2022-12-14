#include "graphics.h"
#include "camera.h"
#include "utilities.h"
#include "game.h"
#include "player.h"
#include "duck.h"
#include "egg.h"
#include "ui.h"
#include "eggcarton.h"

//#define TESTLOAD

/**
 * Key definitions
 * F1: toggle wireframe
 * F2: toggle collision boxes
 * F3: clear debug output
 * F4: toggle debug out of sight
 * F5: duckwalk to cursor
 * F6: toggle action list
 * F7: destroy all ducks
 * F11: toggle fullscreen
 */

int main() {
    sf::RenderWindow window;
    Camera::tieRenderWindow(window);
    Graphics::setRenderWindow(window);
    Graphics::createWindow(false);
    Graphics::loadFont(0, "yourStar.ttf");
    Graphics::loadFont(1, "CascadiaCode.ttf");
    Graphics::loadFont(2, "Aero Matics Regular.ttf");
    Graphics::loadFont(3, "Barlow-SemiBold.ttf");
    FramerateCounter fc;

    Game game;

#ifndef TESTLOAD
    std::shared_ptr<Player> player(new Player(&game));
    player->id = "player$player";
    player->opacity = 1.;
    auto &player_e = game.insertEntity(player);
    game.setPlayer(player_e);
//
//    for (int i = 0; i < 100; i++) {
//        std::shared_ptr<Egg> egg(new Egg(&game));
//        egg->id = game.newId(EGG);
//        egg->opacity = 1.;
//        egg->position.x = -4.5 + i % 10;
//        egg->position.y = -4.5 + i / 10;
//        egg->genderIsMale = i & 0b1;
//        egg->fertilized = true;
//        game.insertEntity(egg);
//    }

    {
        std::shared_ptr<EggCarton> eggcarton(new EggCarton(&game));
        eggcarton->id = game.newId(EGG_CARTON);
        eggcarton->opacity = 1.;
        eggcarton->position.x = 1.;
        eggcarton->position.y = 0.;
        auto cptr = game.insertEntity(eggcarton);

        for (int i = 0; i < 10; i++) {
            std::shared_ptr<Egg> egg(new Egg(&game));
            egg->id = game.newId(EGG);
            egg->opacity = 1.;
            egg->genderIsMale = i & 0b1;
            egg->fertilized = true;
            auto eptr = game.insertEntity(egg);

            {
                Action a(eptr, Timer::getNow(), ENTITY_OWN_BY); // need the container to register "own_by" or else it will not be set up correctly
                a.argEntity[0] = cptr;
                a.argInt[0] = i;
                game.pushAction(a);
            }
        }
    }
    {
        std::shared_ptr<EggCarton> eggcarton(new EggCarton(&game));
        eggcarton->id = game.newId(EGG_CARTON);
        eggcarton->opacity = 1.;
        eggcarton->position.x = 1.;
        eggcarton->position.y = 1.;
        auto cptr = game.insertEntity(eggcarton);

        for (int i = 0; i < 10; i++) {
            std::shared_ptr<Egg> egg(new Egg(&game));
            egg->id = game.newId(EGG);
            egg->opacity = 1.;
            egg->genderIsMale = i & 0b1;
            egg->fertilized = true;
            auto eptr = game.insertEntity(egg);

            {
                Action a(eptr, Timer::getNow() + 1.5 + .2 * i, ENTITY_OWN_BY);
                a.argEntity[0] = cptr;
                a.argInt[0] = i;
                game.pushAction(a);
            }
        }
    }

#else
    game.load();
#endif

    sf::Texture tilemap;
    if (!tilemap.loadFromFile("tilemap.png"))
        debug << "failed to load tilemap.png";
    else
        debug << "maximum texture size: " << sf::Texture::getMaximumSize() << "\n";
    tilemap.setSmooth(true);

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
                case sf::Event::KeyReleased:
                    game.controls.handleKeyPress(event);
                    break;
                case sf::Event::MouseButtonPressed:
                case sf::Event::MouseButtonReleased:
                    game.controls.handleMousePress(event);
                    break;
            }


            if (event.type == sf::Event::Closed)
                if (event.type == sf::Event::Resized)
                    if (event.type == sf::Event::KeyPressed) {
                    }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Right) {
                }
                if (event.key.code == sf::Keyboard::Left) {
                }
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                // mouseWheelPosition += event.mouseWheel.delta;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
            }
        }

        window.clear(sf::Color(129, 214, 131));

        UIRect rectWindow(sf::FloatRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
//        UIRect rectWindow(sf::FloatRect(window.getView().getSize().x / 8., window.getView().getSize().y / 8., window.getView().getSize().x / 4. * 3.,
//                                        window.getView().getSize().y / 4. * 3.));
        Camera::setViewport(rectWindow);


        UIVec moveVec; // on screen vec
        if (game.controls.dirPadPress & 0b0001)
            moveVec.y -= 1.;
        if (game.controls.dirPadPress & 0b0010)
            moveVec.y += 1.;
        if (game.controls.dirPadPress & 0b0100)
            moveVec.x -= 1.;
        if (game.controls.dirPadPress & 0b1000)
            moveVec.x += 1.;
        if (moveVec.len(UIVec()) > .1)
            game.player->slideVelocity = coord(Camera::getTransform().getInverse().transformPoint((moveVec).getVec2f())).unit() * 4.;
        else
            game.player->slideVelocity = coord();

        game.player->heading = game.player->position.angle(Camera::getMouseCoord());

        game.update();
        Graphics::clearQuadsArray();
        game.render();

        Graphics::renderQuads(window, tilemap, Camera::getViewport());

        Graphics::clearQuadsArray();
        game.ui.renderUI();
        Graphics::renderQuads(window, tilemap, Camera::getViewport());

//        Graphics::setFont(1);
//        Graphics::drawText(toStr(fc.getFramerateAndUpdate()) + "fps", sf::Color::Black, 12, UIVec(6, 17), 0., sf::Color(255, 255, 255, 200), 2.);

        Camera::setCenter(Camera::getCenter() + (game.player->position - Camera::getCenter()) * 0.08);

//        Graphics::drawRect(sf::Color(0, 0, 0, 100), -5, rectWindow.pos, rectWindow.pos + rectWindow.size);
//        Graphics::drawText(
//                "[viewport] rectWindow (" + toStr(rectWindow.size.x) + "x" + toStr(rectWindow.size.y) + ") @ " + toStr(rectWindow.pos.x) + ", " + toStr(rectWindow.pos.y),
//                sf::Color::White, 16, rectWindow.pos + UIVec(0, -10), 0., sf::Color::Black, 1.);



        window.display();
    }
    game.save();

    return 0;
}