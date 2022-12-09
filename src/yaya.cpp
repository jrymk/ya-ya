#include "graphics.h"
#include "camera.h"
#include "utilities.h"
#include "game.h"
#include "player.h"

/**
 * Key definitions
 * F1: toggle wireframe
 * F2: reset graph min max
 * F3: clear debug output
 * F4: toggle debug out of sight
 * F5: lay eggs
 * F6: toggle action list
 * F11: toggle fullscreen
 */

int main() {
    sf::RenderWindow window;
    Camera::tieRenderWindow(window);
    Graphics::setRenderWindow(window);
    Graphics::createWindow(false);
    Graphics::loadFont(0, "yourStar.ttf");
    Graphics::loadFont(1, "CascadiaCode.ttf");
    FramerateCounter fc;

    debugGraphs.push_back(DebugGraph("entities", 200, 150, 10000));
    debugGraphs.push_back(DebugGraph("actions", 200, 150, 10000));
    debugGraphs.push_back(DebugGraph("quads", 200, 150, 10000));
    debugGraphs.push_back(DebugGraph("update time", 200, 150, 10000));
    debugGraphs.push_back(DebugGraph("entities(long)", 200, 150, 1000000));

    Game game;

    std::shared_ptr<Player> player(new Player);
    player->id = "player$player";
    player->childClassPtr = player;
    auto& player_e = game.insertEntity(player);
    game.setPlayer(player_e);

    for (int i = 0; i < 4; i++){
        std::shared_ptr<Duck> duck(new Duck);
        duck->id = game.newId(DUCK);
        duck->childClassPtr = duck;
        game.insertEntity(duck);
        duck->position.x = i;
        duck->position.y = 2.;
        duck->genderIsMale = true;
    }

    // game.load();
    
    sf::Texture tilemap;
    if (!tilemap.loadFromFile("tilemap.png"))
        debug << "failed to load tilemap.png";
    else 
        debug << "maximum texture size: " << sf::Texture::getMaximumSize() << "\n";
    tilemap.setSmooth(true);


    while (window.isOpen()) {
        Profiler::timeSplit("event");
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) 
                Graphics::resizeView(event.size.width, event.size.height);
            if (event.type == sf::Event::KeyPressed) {
                game.controls.handleKeyPress(event.key.code);
                
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
//                if (event.mouseButton.button == sf::Mouse::Button::Left) {
//                    for (auto duck : game.entities) {
//                        if (duck.second->type == DUCK) {
//                            Action a(duck.second, Timer::getNow(), DUCK_DUCKWALK_TO_UNTIL);
//                            a.argCoord[0].x = Camera::getMouseCoord().x + (getRand() - .5);
//                            a.argCoord[0].y = Camera::getMouseCoord().y + (getRand() - .5);
//                            game.pushAction(a);
//                        }
//                    }
//                    // for (auto duck : game.ducks)
//                    //     duck.second->actions.push_back(Action(Timer::getNow(), "duckwalk_to_until " + toStr(Camera::getMouseCoord().x + (getRand() - .5)) + " " + toStr(Camera::getMouseCoord().y + (getRand() - .5))));
//                }
            }
        }

        Profiler::timeSplit("clear window");
        // handleEvents(window);
        window.clear(sf::Color(129, 214, 131));

        UIRect rectWindow(sf::FloatRect(0, 0, window.getView().getSize().x, window.getView().getSize().y));
        // UIRect rectWindow(sf::FloatRect(window.getView().getSize().x / 8., window.getView().getSize().y / 8., window.getView().getSize().x / 4. * 3., window.getView().getSize().y / 4. * 3.));
        Camera::setViewport(rectWindow);
        
        Graphics::clearQuadsArray();

        Profiler::timeSplit("move");
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
            game.player->slideVelocity = coord(Camera::getTransform().getInverse().transformPoint((moveVec).getVec2f())) / coord(Camera::getTransform().getInverse().transformPoint((moveVec).getVec2f())).len() * 4.;
        else
            game.player->slideVelocity = coord();

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        //     player->velocity = 3.;
        // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        //     player->velocity = -2.;
        // else
        //     player->velocity = 0.;

        player->heading = player->position.angle(Camera::getMouseCoord());
        
        // player->heading += double(sf::Mouse::getPosition(window).x) / -200.;
        // sf::Mouse::setPosition(sf::Vector2i(0, sf::Mouse::getPosition(window).y), window);
        Profiler::timeSplit("update");
        game.update();
        Profiler::timeSplit("gamerender");
        game.render();

        Profiler::timeSplit("render");
        Graphics::renderQuads(window, tilemap, Camera::getViewport());

        
        debugGraphs[0].newGraphEntry(game.entities.size());
        debugGraphs[1].newGraphEntry(game.actionList.size());
        debugGraphs[2].newGraphEntry(Graphics::getQuadCount());
        debugGraphs[3].newGraphEntry(game.updateTime);
        debugGraphs[4].newGraphEntry(game.entities.size());
        
        Profiler::timeSplit("post");
        Graphics::setFont(1);
        Graphics::drawText(toStr(fc.getFramerateAndUpdate()) + "fps", sf::Color::White, 24, UIVec(10, 30), 0., sf::Color::Black, 4.);

        Camera::setCenter(Camera::getCenter() + (player->position - Camera::getCenter()) * 0.08);

        Camera::printCameraInfo();

        // Graphics::drawRect(sf::Color(0, 0, 0, 100), -5, rectWindow.pos, rectWindow.pos + rectWindow.size);
        // Graphics::drawText("[viewport] rectWindow (" + toStr(rectWindow.size.x) + "x" + toStr(rectWindow.size.y) + ") @ " + toStr(rectWindow.pos.x) + ", " + toStr(rectWindow.pos.y), 
        //     sf::Color::White, 16, rectWindow.pos + UIVec(0, -10), 0., sf::Color::Black, 1.);

        Profiler::timeSplit("renderdebugoverlay");
        renderDebugOverlay(window);
        Profiler::drawBarAndClear();
        Profiler::timeSplit("display");
        window.display();
    }
    game.save();

    return 0;
}