#include "utilities.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "graphics.hpp"
#include "debugger.hpp"
#include "events.hpp"

/**
 * Key definitions
 * F1: show wireframe
 * F2: reset graph min max
 * F3: clear debug output
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

    Entity player;

    sf::Texture tilemap;
    if (!tilemap.loadFromFile("tilemap.png"))
        debug << "failed to load tilemap.png";
    tilemap.setSmooth(true);

    while (window.isOpen()) {
        handleEvents(window);
        window.clear();

        UIRect rectWindow(sf::FloatRect(0., 0., window.getView().getSize().x, window.getView().getSize().y));

        Graphics::setFont(1);
        Graphics::drawText(toString(fc.getFramerateAndUpdate()) + "", sf::Color::White, 24, rectWindow * UIVec(.0, .0) + UIVec(10, 30), 0.);

        Camera::setViewport(rectWindow);
        Camera::setCenter(Camera::getCenter() + (player.position - Camera::getCenter()) * 0.15);
        Camera::printCameraInfo();

        for (int x = int(Camera::getCenter().x - 100) / 5 * 5; x <= int(Camera::getCenter().x + 100) / 5 * 5; x+=5) {
            for (int y = int(Camera::getCenter().y - 40) / 5 * 5; y <= int(Camera::getCenter().y + 40) / 5 * 5; y+=5) {
                Graphics::drawRect(sf::Color(90, 90, 90), 2, Camera::getScreenPos({x, y}), Camera::getScreenPos({x + 0.01, y + 0.01}));
            }
        }

        sf::VertexArray tiles(sf::PrimitiveType::Quads, 4);
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(0., 1.)).getVec2f(), sf::Vector2f(0, 90.5)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(1., 1.)).getVec2f(), sf::Vector2f(384, 0)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(1., 0.)).getVec2f(), sf::Vector2f(512, 271.5)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(0., 0.)).getVec2f(), sf::Vector2f(128, 362)));

        window.draw(tiles, &tilemap);


        // player.render();

        renderDebugOverlay(window);
        window.display();
    }

    return 0;
}