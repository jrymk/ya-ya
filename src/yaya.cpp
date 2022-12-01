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
    Timer::setGlobalStartTimepoint();
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
    else 
        debug << "maximum texture size: " << sf::Texture::getMaximumSize() << "\n";
    tilemap.setSmooth(true);

    while (window.isOpen()) {
        handleEvents(window);
        window.clear(sf::Color(129, 214, 131));

        UIRect rectWindow(sf::FloatRect(0., 0., window.getView().getSize().x, window.getView().getSize().y));

        Graphics::setFont(1);
        Graphics::drawText(toString(fc.getFramerateAndUpdate()) + "", sf::Color::White, 24, rectWindow * UIVec(.0, .0) + UIVec(10, 30), 0.);

        Camera::setViewport(rectWindow);
        Camera::setCenter(Camera::getCenter() + (player.position - Camera::getCenter()) * 0.15);
        Camera::printCameraInfo();

        sf::VertexArray tiles(sf::PrimitiveType::Quads, 0);

        for (int x = int(Camera::getCenter().x - 40); x <= int(Camera::getCenter().x + 40); x+=1) {
            for (int y = int(Camera::getCenter().y - 30) / 2 * 2; y <= int(Camera::getCenter().y + 30); y+=1) {
                tiles.append(sf::Vertex(Camera::getScreenPos(coord(x, y)).getVec2f(), sf::Color::Red, sf::Vector2f(0, 90.5)));
                tiles.append(sf::Vertex(Camera::getScreenPos(coord(x + 1., y)).getVec2f(), sf::Color::Yellow, sf::Vector2f(384, 0)));
                tiles.append(sf::Vertex(Camera::getScreenPos(coord(x + 1., y - 1.)).getVec2f(), sf::Color::Blue, sf::Vector2f(512, 271.5)));
                tiles.append(sf::Vertex(Camera::getScreenPos(coord(x, y - 1.)).getVec2f(), sf::Color::Green, sf::Vector2f(128, 362)));

                tiles.append(sf::Vertex(Camera::getScreenPos(coord(x, y + 1.)).getVec2f(), sf::Color::Green, sf::Vector2f(0 + 128, 90.5 + 90.5 * 3)));
                tiles.append(sf::Vertex(Camera::getScreenPos(coord(x + 1., y + 1.)).getVec2f(), sf::Color::Blue, sf::Vector2f(384 + 128, 0 + 90.5 * 3)));
                tiles.append(sf::Vertex(Camera::getScreenPos(coord(x + 1., y)).getVec2f(), sf::Color::Yellow, sf::Vector2f(512 + 128, 271.5 + 90.5 * 3)));
                tiles.append(sf::Vertex(Camera::getScreenPos(coord(x, y)).getVec2f(), sf::Color::Red, sf::Vector2f(128 + 128, 362 + 90.5 * 3)));
            }
        }

        tiles.append(sf::Vertex(Camera::getScreenPos(coord(0., 1.)).getVec2f(), sf::Vector2f(0 + 128 * 4, 90.5 + 90.5 * 2)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(1., 1.)).getVec2f(), sf::Vector2f(384 + 128 * 4, 0 + 90.5 * 2)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(1., 0.)).getVec2f(), sf::Vector2f(512 + 128 * 4, 271.5 + 90.5 * 2)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(0., 0.)).getVec2f(), sf::Vector2f(128 + 128 * 4, 362 + 90.5 * 2)));

        tiles.append(sf::Vertex(Camera::getScreenPos(coord(0., 2.)).getVec2f(), sf::Vector2f(0, 90.5)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(1., 2.)).getVec2f(), sf::Vector2f(384, 0)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(1., 1.)).getVec2f(), sf::Vector2f(512, 271.5)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(0., 1.)).getVec2f(), sf::Vector2f(128, 362)));

        tiles.append(sf::Vertex(Camera::getScreenPos(coord(0., 1.)).getVec2f(), sf::Vector2f(0 + 128, 90.5 + 90.5 * 3)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(1., 1.)).getVec2f(), sf::Vector2f(384 + 128, 0 + 90.5 * 3)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(1., 0.)).getVec2f(), sf::Vector2f(512 + 128, 271.5 + 90.5 * 3)));
        tiles.append(sf::Vertex(Camera::getScreenPos(coord(0., 0.)).getVec2f(), sf::Vector2f(128 + 128, 362 + 90.5 * 3)));

        window.draw(tiles, &tilemap);

        Camera::setZoom(std::pow(2, 5. + 2. * std::cos((float)Timer::getElapsedMs(Timer::globalStart) / 7400. * 6.28)));
        Camera::setCenter(coord(
            2. * std::cos((float)Timer::getElapsedMs(Timer::globalStart) / 2100. * 6.28),
            2. * std::sin((float)Timer::getElapsedMs(Timer::globalStart) / 3200. * 6.28)
        ));
        // player.render();

        renderDebugOverlay(window);
        window.display();
    }

    return 0;
}