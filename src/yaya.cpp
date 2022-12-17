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

#define TESTSAVE
#define TESTLOAD
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
 * F11: toggle fullscreen
 */

int main() {
    sf::RenderWindow window;
    Camera::tieRenderWindow(window);
    Graphics::setRenderWindow(window);
    Graphics::createWindow(false);
    Graphics::loadFont(0, "./res/yourStar.ttf");
    Graphics::loadFont(1, "./res/CascadiaCode.ttf");
    Graphics::loadFont(2, "./res/Aero Matics Regular.ttf");
    Graphics::loadFont(3, "./res/Barlow-SemiBold.ttf");
    Graphics::loadFont(4, "./res/SourceHanSansTC-Bold.otf");
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
    std::shared_ptr<Truck> truck(new Truck(&game));
    truck->id = game.newId(TRUCK);
    truck->opacity = 1.;
    truck->collisionPushable = false;
    truck->underlyingPos.x = 6.5;
    truck->underlyingPos.y = 0;
    auto tptr = game.insertEntity(truck);
    for (int x = 3; x < 10; x++) {
        for (int y = -2; y < 2; y++)
            game.map.getTile(x, y).setTileType(Map::Tile::TRUCK); // for collision, because I don't want to make collision boxes work with entities
    }

    {
        std::shared_ptr<EggCarton> eggcarton(new EggCarton(&game));
        eggcarton->id = game.newId(EGG_CARTON);
        eggcarton->opacity = 1.;
        eggcarton->underlyingPos.x = 1.;
        eggcarton->underlyingPos.y = 1.;
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
    if (!tilemap.loadFromFile("./res/tilemap.png"))
        debug << "failed to load tilemap.png";
    else
        debug << "maximum texture size: " << sf::Texture::getMaximumSize() << "\n";
    tilemap.setSmooth(true);

    std::vector<Audio> audioVec;
    Audio audio;
    audio.loadSound("./res/walk.wav");
    audioVec.push_back(audio);
    audio.loadSound("./res/tick.wav");
    audioVec.push_back(audio);

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
                    game.controls.handleSoundOnAction(event, audioVec[0]);
                    break;
                case sf::Event::MouseButtonPressed:
                case sf::Event::MouseButtonReleased:
                    game.controls.handleMousePress(event);
                    game.controls.handleSoundOnAction(event, audioVec[1]);
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
                game.ui.displayScaling += event.mouseWheel.delta * .25;
                game.ui.displayScaling = std::max(game.ui.displayScaling, .25f);
                Camera::setZoom(15. / game.ui.displayScaling);
                debug << "Display scaling: " << game.ui.displayScaling << "\n";
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

        Graphics::insertUserWireframe(
                Camera::getScreenPos(game.player->underlyingPos) + UIVec(5, 0),
                Camera::getScreenPos(game.player->underlyingPos) + UIVec(0, 5),
                Camera::getScreenPos(game.player->underlyingPos) + UIVec(-5, 0),
                Camera::getScreenPos(game.player->underlyingPos) + UIVec(0, -5),
                sf::Color(100, 255, 255, int(std::max(std::min((game.player->underlyingPos.len(game.player->position) - .5) * 255., 255.), 0.))),
                sf::Color(0, 0, 0, int(std::max(std::min((game.player->underlyingPos.len(game.player->position) - .5) * 255., 255.), 0.)))
        );
        Graphics::insertUserWireframe(
                Camera::getScreenPos(game.player->underlyingPos) + UIVec(3, 0),
                Camera::getScreenPos(game.player->underlyingPos) + UIVec(0, 3),
                Camera::getScreenPos(game.player->underlyingPos) + UIVec(-3, 0),
                Camera::getScreenPos(game.player->underlyingPos) + UIVec(0, -3),
                sf::Color(0, 0, 0, int(std::max(std::min((game.player->underlyingPos.len(game.player->position) - .5) * 255., 255.), 0.))),
                sf::Color(100, 255, 255, int(std::max(std::min((game.player->underlyingPos.len(game.player->position) - .5) * 255., 255.), 0.)))
        );

        game.controls.update();
        // this should be AFTER game.update() so when the next main loop comes, the controls event are polled and executed, the states are up to date
        // but BEFORE renderUI() so it gets the newest states

        Graphics::renderQuads(window, tilemap, Camera::getViewport());

        Graphics::clearQuadsArray();
        game.ui.renderUI();
        Graphics::renderQuads(window, tilemap, Camera::getViewport());

//        Graphics::setFont(1);
//        Graphics::drawText(toStr(fc.getFramerateAndUpdate()) + "fps", sf::Color::Black, 12, UIVec(6, 17), 0., sf::Color(255, 255, 255, 200), 2.);

        Camera::setCenter(Camera::getCenter() + (game.player->position - Camera::getCenter()) * 0.1); /// TODO: decide underlying pos or pos

//        Graphics::drawRect(sf::Color(0, 0, 0, 100), -5, rectWindow.pos, rectWindow.pos + rectWindow.size);
//        Graphics::drawText(
//                "[viewport] rectWindow (" + toStr(rectWindow.size.x) + "x" + toStr(rectWindow.size.y) + ") @ " + toStr(rectWindow.pos.x) + ", " + toStr(rectWindow.pos.y),
//                sf::Color::White, 16, rectWindow.pos + UIVec(0, -10), 0., sf::Color::Black, 1.);


        window.display();
    }
    
    #ifdef TESTSAVE
        game.save();
    #endif
    #ifdef SUSPEND_CMD
        getchar();  // for testing with cmd
    #endif
    return 0;
}