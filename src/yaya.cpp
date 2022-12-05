#include "utilities.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "graphics.hpp"
#include "debugger.hpp"
#include "events.hpp"
#include "duck.hpp"
#include "player.hpp"
#include "game.hpp"

/**
 * Key definitions
 * F1: toggle wireframe
 * F2: reset graph min max
 * F3: clear debug output
 * F4: toggle debug out of sight
 * F5: lay eggs
 * F11: toggle fullscreen
 */

int main() {
    srand(time(0));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window;
    window.create(sf::VideoMode(1600, 1200), L"Ya-Ya!", sf::Style::Default, settings);
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(true);
    Graphics::setRenderWindow(window);
    Graphics::loadFont(0, "yourStar.ttf");
    Graphics::loadFont(1, "CascadiaCode.ttf");
    FramerateCounter fc;

    debugGraphs.push_back(DebugGraph("entities", 200, 150, 10000));
    debugGraphs.push_back(DebugGraph("actions", 200, 150, 10000));
    debugGraphs.push_back(DebugGraph("quads", 200, 150, 10000));
    debugGraphs.push_back(DebugGraph("update time", 200, 150, 10000));

    Game game;
    
    Player* player = new Player();
    player->id = "player$player";
    game.insertEntity(player);

    // for (int i = 0; i < 20; i++) {
    //     for (int j = 0; j < 20; j++) {
    //         if (i >= 9 && i <= 11 && j >= 9 && j <= 11)
    //             continue;
    //         Duck* duck = new Duck();
    //         duck->position.x = float(i - 10);
    //         duck->position.y = float(j - 10);
    //         game.entities.push_back(duck);
    //         game.ducks.push_back(duck);
    //     }
    // }
    {
        Duck* duck = new Duck();
        duck->id = game.newId("duck");
        duck->position.x = 2.;
        duck->position.y = 2.;
        duck->genderIsMale = true;
        game.insertEntity(duck);
    }
    {
        Duck* duck = new Duck();
        duck->id = game.newId("duck");
        duck->position.x = 2.;
        duck->position.y = -2.;
        duck->genderIsMale = false;
        game.insertEntity(duck);
    }
    {
        Duck* duck = new Duck();
        duck->id = game.newId("duck");
        duck->position.x = -2.;
        duck->position.y = -2.;
        duck->genderIsMale = true;
        game.insertEntity(duck);
    }
    {
        Duck* duck = new Duck();
        duck->id = game.newId("duck");
        duck->position.x = -2.;
        duck->position.y = 2.;
        duck->genderIsMale = false;
        game.insertEntity(duck);
    }

    // game.load();
    
    sf::Texture tilemap;
    if (!tilemap.loadFromFile("tilemap.png"))
        debug << "failed to load tilemap.png";
    else 
        debug << "maximum texture size: " << sf::Texture::getMaximumSize() << "\n";
    tilemap.setSmooth(true);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) {
                debug << "Resized to " << event.size.width << "*" << event.size.height << "\n";
                window.setSize(sf::Vector2u(event.size.width, event.size.height));
                sf::View view = window.getView();
                view.setSize(sf::Vector2f(event.size.width, event.size.height));
                view.setCenter(sf::Vector2f(event.size.width / 2., event.size.height / 2.));
                window.setView(view);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F1) {
                    Graphics::showWireframe = !Graphics::showWireframe;
                    debug << "Toggled wireframe: " << Graphics::showWireframe << "\n";
                }
                if (event.key.code == sf::Keyboard::F2) {
                    for (int i = 0; i < debugGraphs.size(); i++)
                        debugGraphs[i].resetRange();
                }
                if (event.key.code == sf::Keyboard::F3) {
                    debugStream.str("");
                }
                if (event.key.code == sf::Keyboard::F4) {
                    Graphics::debugOutOfSight = !Graphics::debugOutOfSight;
                    debug << "Toggled debug out of sight: " << Graphics::debugOutOfSight << "\n";
                }
                if (event.key.code == sf::Keyboard::F5) {
                    for (auto duck : game.entities) {
                        auto split = splitId(duck.first);
                        if (split.first == "duck")
                            game.pushAction(duck.first, Timer::getNow() + (getRand() * 10.), "lay_fertilized_egg");
                    }
                    debug << "Lay eggs\n";
                }
                if (event.key.code == sf::Keyboard::F11) {
                    if (!graphicsIsFullscreen) {
                        graphicsIsFullscreen = true;
                        window.close();
                        sf::ContextSettings settings;
                        settings.antialiasingLevel = 4;
                        window.create(sf::VideoMode::getFullscreenModes()[0], L"Ya-Ya!", sf::Style::Fullscreen, settings);
                        window.setVerticalSyncEnabled(true);
                        window.setKeyRepeatEnabled(false);
                    }
                    else {
                        graphicsIsFullscreen = false;
                        window.close();
                        sf::ContextSettings settings;
                        settings.antialiasingLevel = 4;
                        window.create(sf::VideoMode(1600, 900), L"Ya-Ya!", sf::Style::Default, settings);
                        window.setVerticalSyncEnabled(true);
                        window.setKeyRepeatEnabled(false);
                    }
                    debug << "Toggled fullscreen: " << graphicsIsFullscreen << "\n";
                }
                if (event.key.code == sf::Keyboard::Right) {
                }
                if (event.key.code == sf::Keyboard::Left) {
                }
                if (event.key.code == sf::Keyboard::P) {
                    updatePhysics = true;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Right) {
                }
                if (event.key.code == sf::Keyboard::Left) {
                }
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                mouseWheelPosition += event.mouseWheel.delta;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    for (auto duck : game.entities) {
                        auto split = splitId(duck.first);
                        if (split.first == "duck")
                            game.pushAction(duck.first, Timer::getNow(), "duckwalk_to_until " + toStr(Camera::getMouseCoord(window).x + (getRand() - .5)) + " " + toStr(Camera::getMouseCoord(window).y + (getRand() - .5)));
                    }
                    // for (auto duck : game.ducks) 
                    //     duck.second->actions.push_back(Action(Timer::getNow(), "duckwalk_to_until " + toStr(Camera::getMouseCoord(window).x + (getRand() - .5)) + " " + toStr(Camera::getMouseCoord(window).y + (getRand() - .5))));
                }

            }
        }

        // handleEvents(window);
        window.clear(sf::Color(129, 214, 131));

        UIRect rectWindow(sf::FloatRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
        // UIRect rectWindow(sf::FloatRect(window.getView().getSize().x / 8., window.getView().getSize().y / 8., window.getView().getSize().x / 4. * 3., window.getView().getSize().y / 4. * 3.));
        Camera::setViewport(rectWindow);
        
        Graphics::clearQuadsArray();

        // for (int x = int(Camera::getCenter().x - 40); x <= int(Camera::getCenter().x + 40); x+=1) {
        //     for (int y = int(Camera::getCenter().y - 30) / 2 * 2; y <= int(Camera::getCenter().y + 30); y+=1) {
        //         Graphics::insertQuad(
        //             Graphics::Quad(
        //                 0.2,
        //                 Camera::getScreenPos(coord(x, y)), sf::Vector2f(0, 90.5), sf::Color::Red,
        //                 Camera::getScreenPos(coord(x + 1, y)), sf::Vector2f(384, 0), sf::Color::Yellow,
        //                 Camera::getScreenPos(coord(x + 1, y - 1.)), sf::Vector2f(512, 271.5), sf::Color::Blue,
        //                 Camera::getScreenPos(coord(x, y- 1.)), sf::Vector2f(128, 362), sf::Color::Green
        //             )
        //         );
        //         Graphics::insertQuad(
        //             Graphics::Quad(
        //                 0.2,
        //                 Camera::getScreenPos(coord(x, y+ 1.)), sf::Vector2f(0 + 128, 90.5 + 90.5 * 3), sf::Color::Green,
        //                 Camera::getScreenPos(coord(x + 1, y + 1.)), sf::Vector2f(384 + 128, 0 + 90.5 * 3), sf::Color::Blue,
        //                 Camera::getScreenPos(coord(x + 1, y)), sf::Vector2f(512 + 128, 271.5 + 90.5 * 3), sf::Color::Yellow,
        //                 Camera::getScreenPos(coord(x, y)), sf::Vector2f(128 + 128, 362 + 90.5 * 3), sf::Color::Red
        //             )
        //         );
        //     }
        // }

        UIVec moveVec;        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            moveVec.y -= 1.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            moveVec.y += 1.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            moveVec.x -= 1.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            moveVec.x += 1.;
        if (moveVec.len(UIVec()) > .1)
            player->slideVelocity = coord(Camera::getTransform().getInverse().transformPoint((moveVec).getVec2f())) / coord(Camera::getTransform().getInverse().transformPoint((moveVec).getVec2f())).len() * 4.;
        else 
            player->slideVelocity = coord();

        // for (auto duck : game.ducks) {
        //     if (Camera::getMouseCoord(window).len(duck->position) < .6) {
        //         // duck->heading = Camera::getMouseCoord(window).angle(duck->position);
        //         // duck->velocity = 2. / Camera::getMouseCoord(window).len(duck->position);
        //         if (duck->zPosition == 0.) {
        //             duck->insertAction(Timer::getNow(), "duckwalk_to_until 0 0");
        //         }
        //         //     duck->zVelocity = .2;
        //     }
        //     else if (player->position.len(duck->position) < .6) {
        //         duck->heading = player->position.angle(duck->position);
        //         duck->velocity = 2. / player->position.len(duck->position);
        //         // if (duck->zPosition == 0.)
        //         //     duck->zVelocity = .2;
        //     }
        //     else {
        //         // duck->heading = 0.;
        //         duck->velocity = duck->velocity * 0.9;
        //     }
        // }
        for (auto e : game.entities) {
            auto eid = splitId(e.first);
            if (eid.first != "duck" && eid.first != "player")
                continue;
            
            auto result = game.neighborsFinder.findNeighbors(e.second->position, .3, "duck");
            
            // if (player->position.len(e.second->position) < .6) {
            //     e.second->heading = player->position.angle(e.second->position);
            //     e.second->velocity = 2. / player->position.len(e.second->position);
            //     // if (duck->zPosition == 0.)
            //     //     duck->zVelocity = .2;
            // }
            for (auto f : result) {
                if (e.second == f)
                    continue;
                if (f->type != "duck")
                    continue;

                if (e.second->position.len(f->position) < .3) {
                    game.pushAction("global", Timer::getNow(), "process_collision " + e.second->id + " " + f->id);
                }
            }
        }


        player->heading = player->position.angle(Camera::getMouseCoord(window));
        
        game.update();
        game.render();

        Graphics::renderQuads(window, tilemap, Camera::getViewport());

        Entity* closest = nullptr;
        for (auto e : game.entities) {
            if ((closest == nullptr && Camera::getScreenPos(e.second->position).len(Camera::getMousePos(window)) < 20.)
                 || (closest != nullptr && Camera::getScreenPos(e.second->position).len(Camera::getMousePos(window)) < Camera::getScreenPos(closest->position).len(Camera::getMousePos(window)))) {
                closest = e.second;
            }
        }
        if (closest)
            Graphics::drawText(closest->getDescriptionStr(), sf::Color::Black, 12., Camera::getMousePos(window) + UIVec(0., 40.), 0., sf::Color(255, 255, 255, 140), 3.);
        
        debugGraphs[0].newGraphEntry(game.entities.size());
        debugGraphs[1].newGraphEntry(game.actionList.size());
        debugGraphs[2].newGraphEntry(Graphics::getQuadCount());
        debugGraphs[3].newGraphEntry(game.updateTime);

        Graphics::setFont(1);
        Graphics::drawText(toStr(fc.getFramerateAndUpdate()) + "fps", sf::Color::White, 24, UIVec(10, 30), 0., sf::Color::Black, 4.);

        Camera::setCenter(Camera::getCenter() + (player->position - Camera::getCenter()) * 0.08);

        Camera::printCameraInfo(window);

        Graphics::drawRect(sf::Color(0, 0, 0, 100), -5, rectWindow.pos, rectWindow.pos + rectWindow.size);
        Graphics::drawText("[viewport] rectWindow (" + toStr(rectWindow.size.x) + "x" + toStr(rectWindow.size.y) + ") @ " + toStr(rectWindow.pos.x) + ", " + toStr(rectWindow.pos.y), 
            sf::Color::White, 16, rectWindow.pos + UIVec(0, -10), 0., sf::Color::Black, 1.);

        renderDebugOverlay(window);
        window.display();
    }
    game.save();

    return 0;
}