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

    debugGraphs.push_back(DebugGraph("camera", 300, 150, 1500));
    debugGraphs.push_back(DebugGraph("force", 300, 150, 1500));
    debugGraphs.push_back(DebugGraph("acceleration", 300, 150, 1500));
    debugGraphs.push_back(DebugGraph("velocity", 300, 150, 1500));
    debugGraphs.push_back(DebugGraph("position", 300, 150, 1500));
    // debugGraphs.push_back(DebugGraph(200, 150));

    Entity player;

    std::deque<std::pair<std::chrono::steady_clock::time_point, coord>> trail;

    while (window.isOpen()) {
        handleEvents(window);
        window.clear();

        UIRect rectWindow(sf::FloatRect(0., 0., window.getView().getSize().x, window.getView().getSize().y));

        Graphics::setFont(1);
        Graphics::drawText(toString(fc.getFramerateAndUpdate()) + "", sf::Color::White, 24, rectWindow * UIVec(.0, .0) + UIVec(10, 30), 0.);
        // Graphics::setFont(0);
        // Graphics::drawText("The quick 123 gg", sf::Color::White, 48, rectWindow * UIVec(.5, .5) + UIVec(0, 0), 0.5);

        Camera::setViewport(rectWindow);
        Camera::setCenter(Camera::getCenter() + (player.position - Camera::getCenter()) * 0.15);
        Camera::printCameraInfo();

        for (int x = int(Camera::getCenter().x - 100) / 5 * 5; x <= int(Camera::getCenter().x + 100) / 5 * 5; x+=5) {
            for (int y = int(Camera::getCenter().y - 40) / 5 * 5; y <= int(Camera::getCenter().y + 40) / 5 * 5; y+=5) {
                Graphics::drawRect(sf::Color(90, 90, 90), 2, Camera::getScreenPos({x, y}), Camera::getScreenPos({x + 0.01, y + 0.01}));
            }
        }

        // Graphics::drawRect(sf::Color::White, 2, Camera::getScreenPos({mrSquarePos, 0.}), Camera::getScreenPos({mrSquarePos + 3.,3.}));
        // Graphics::drawText(toString(mrSquarePos, 3), sf::Color::White, 16, Camera::getScreenPos({mrSquarePos + 5 ,5.}) + UIVec(0, -5.), 1.);

        player.force.x = 0.;
        player.force.y = 0.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.force.x += 0.90225 * 25;
            player.force.y += -0.37593 * 25;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player.force.x += -0.90225 * 25;
            player.force.y += 0.37593 * 25;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player.force.x += 0.75187 * 25;
            player.force.y += 1.35338 * 25;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.force.x += -0.75187 * 25;
            player.force.y += -1.35338 * 25;
        }


        // debugGraphs[1].newGraphEntry(mrSquarePos);
        // if ((player.position - Camera::getCenter()).len() > 10)

        debugGraphs[0].newGraphEntry(Camera::getCenter().x);
        debugGraphs[1].newGraphEntry(player.force.len());
        debugGraphs[2].newGraphEntry(player.acceleration.len());
        debugGraphs[3].newGraphEntry(player.velocity.len());
        debugGraphs[4].newGraphEntry(player.position.x);

        player.updatePhysics();
        // Graphics::drawLine(sf::Color::White, 4, rectWindow * UIVec(.5, .5), UIVec(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
        // (mouseWheelPosition);
        trail.push_back(std::make_pair(std::chrono::steady_clock::now(), player.position));
        while (!trail.empty() && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - trail.front().first).count() >= 5000)
            trail.pop_front();
        for (int i = 0; i < trail.size(); i++)
            Graphics::drawRect(sf::Color(100, 0, 150), 2, Camera::getScreenPos(trail[i].second), Camera::getScreenPos(trail[i].second + coord(0.01, 0.01)));

        player.render();

        renderDebugOverlay(window);
        window.display();
    }

    return 0;
}