#include "utilities.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "graphics.hpp"
#include "debugger.hpp"
#include "events.hpp"
#include "duck.hpp"
#include "game.hpp"

/**
 * Key definitions
 * F1: toggle wireframe
 * F2: reset graph min max
 * F3: clear debug output
 * F4: toggle debug out of sight
 * F11: toggle fullscreen
 */

int main() {
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

    debugGraphs.push_back(DebugGraph("Quads", 200, 150, 10000));

    Game game;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            Duck* duck = new Duck();
            duck->position.x = i - 5;
            duck->position.y = j - 5;
            game.ducks.push_back(duck);
        }
    }
    
    
    sf::Texture tilemap;
    if (!tilemap.loadFromFile("tilemap.png"))
        debug << "failed to load tilemap.png";
    else 
        debug << "maximum texture size: " << sf::Texture::getMaximumSize() << "\n";
    tilemap.setSmooth(true);


    while (window.isOpen()) {
        handleEvents(window);
        window.clear(sf::Color(129, 214, 131));

        UIRect rectWindow(sf::FloatRect(window.getView().getSize().x / 8., window.getView().getSize().y / 8., window.getView().getSize().x / 4. * 3., window.getView().getSize().y / 4. * 3.));
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

        for (auto duck : game.ducks) {
            if (Camera::getMouseCoord(window).len(duck->position) < .6) {
                duck->heading = Camera::getMouseCoord(window).angle(duck->position);
                duck->velocity = 2. / Camera::getMouseCoord(window).len(duck->position);
            }
            else {
                // duck->heading = 0.;
                duck->velocity = duck->velocity * 0.9;
            }
        }
        for (auto duck : game.ducks) {
            for (auto ducl : game.ducks) {
                if (duck == ducl)
                    continue;
                if ((ducl->position.len(duck->position) < .5 && duck->velocity < 0.01) || (ducl->position.len(duck->position) < .4)) {
                    duck->heading = ducl->position.angle(duck->position);
                    duck->velocity = 2. / ducl->position.len(duck->position);
                }
                else {
                    // duck->heading = 0.;
                    // duck->velocity = duck->velocity * 0.99;
                }
            }
        }
        
        game.update();
        game.render();

        Graphics::renderQuads(window, tilemap, Camera::getViewport());
        
        debugGraphs[0].newGraphEntry(Graphics::getQuadCount());

        Graphics::setFont(1);
        Graphics::drawText(toString(fc.getFramerateAndUpdate()) + "fps", sf::Color::White, 24, UIVec(10, 30), 0., sf::Color::Black, 4.);

        // Camera::setCenter(Camera::getCenter() + (ducky.position - Camera::getCenter()) * 0.15);
        Camera::printCameraInfo(window);

        Graphics::drawRect(sf::Color(0, 0, 0, 100), -5, rectWindow.pos, rectWindow.pos + rectWindow.size);
        Graphics::drawText("[viewport] rectWindow (" + toString(rectWindow.size.x) + "x" + toString(rectWindow.size.y) + ") @ " + toString(rectWindow.pos.x) + ", " + toString(rectWindow.size.y), 
            sf::Color::White, 16, rectWindow.pos + UIVec(0, -10), 0., sf::Color::Black, 1.);

        renderDebugOverlay(window);
        window.display();
    }

    return 0;
}