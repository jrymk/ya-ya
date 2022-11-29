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
    window.create(sf::VideoMode(1600, 900), L"Ya-Ya!", sf::Style::Default, settings);
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(true);
    Graphics::setRenderWindow(window);
    Graphics::loadFont(0, "yourStar.ttf");
    Graphics::loadFont(1, "CascadiaCode.ttf");
    FramerateCounter fc;

    debugGraphs.push_back(DebugGraph("frametime", 200, 150, 1000));
    debugGraphs.push_back(DebugGraph("Mr.Square pos", 250, 150, 3000));
    debugGraphs.push_back(DebugGraph("camera pos", 250, 150, 3000));
    debugGraphs.push_back(DebugGraph("right status", 250, 150, 3000));
    // debugGraphs.push_back(DebugGraph(200, 150));

    Camera cam;

    double mrSquarePos = 0;
    double mrSquareStart= 0;

    while (window.isOpen()) {
        handleEvents(window);
        window.clear();

        UIRect rectWindow(sf::FloatRect(0., 0., window.getView().getSize().x, window.getView().getSize().y));

        Graphics::setFont(1);
        Graphics::drawText(toString(fc.getFramerateAndUpdate()) + "", sf::Color::White, 24, rectWindow * UIVec(.0, .0) + UIVec(10, 30), 0.);
        debugGraphs[0].newGraphEntry(fc.getFrametimeMs());
        // Graphics::setFont(0);
        // Graphics::drawText("The quick 123 gg", sf::Color::White, 48, rectWindow * UIVec(.5, .5) + UIVec(0, 0), 0.5);

        cam.setViewport(rectWindow);
        cam.printCameraInfo();

        for (int x = int(cam.getCenter().x - 100) / 5 * 5; x <= int(cam.getCenter().x + 100) / 5 * 5; x+=5) {
            for (int y = int(cam.getCenter().y - 40) / 5 * 5; y <= int(cam.getCenter().y + 40) / 5 * 5; y+=5) {
                Graphics::drawRect(sf::Color(90, 90, 90), 2, cam.getScreenPos({x, y}), cam.getScreenPos({x + 0.01, y + 0.01}));
            }
        }

        Graphics::drawRect(sf::Color::White, 2, cam.getScreenPos({mrSquarePos, 0.}), cam.getScreenPos({mrSquarePos + 3.,3.}));
        Graphics::drawText(toString(mrSquarePos, 3), sf::Color::White, 16, cam.getScreenPos({mrSquarePos + 5 ,5.}) + UIVec(0, -5.), 1.);
        
        rightArrowButton.getElapsedMicrosAndUpdate();
        leftArrowButton.getElapsedMicrosAndUpdate();
        float speed = 50.;
        int easeMs = 150;
        if (rightArrowButton.microsSinceStart() <= easeMs * 1000) {
            mrSquarePos = mrSquareStart + speed * 2. * std::pow((float)rightArrowButton.microsSinceStart() / (easeMs * 4000), 2.);
            debugGraphs[3].newGraphEntry(1);
        }
        else if (rightArrowButton.isStarted) {
            mrSquarePos = mrSquareStart + speed * ((float)rightArrowButton.microsSinceStart() / (easeMs * 4000) - 0.125);
            debugGraphs[3].newGraphEntry(2);
        }
        else if (rightArrowButton.microsSinceEnd() <= easeMs * 1000) {
            mrSquarePos = mrSquareStart + speed * (((float)rightArrowButton.microsEvent() / (easeMs * 4000) - 0.125) + 0.125 - 2. * std::pow(((easeMs * 1000.) - (float)rightArrowButton.microsSinceEnd()) / (easeMs * 4000), 2.));
            debugGraphs[3].newGraphEntry(3);
        }
        else if (leftArrowButton.microsSinceStart() <= easeMs * 1000) {
            mrSquarePos = mrSquareStart - speed * 2. * std::pow((float)leftArrowButton.microsSinceStart() / (easeMs * 4000), 2.);
            debugGraphs[3].newGraphEntry(-1);
        }
        else if (leftArrowButton.isStarted) {
            mrSquarePos = mrSquareStart - speed * ((float)leftArrowButton.microsSinceStart() / (easeMs * 4000) - 0.125);
            debugGraphs[3].newGraphEntry(-2);
        }
        else if (leftArrowButton.microsSinceEnd() <= easeMs * 1000) {
            mrSquarePos = mrSquareStart - speed * (((float)leftArrowButton.microsEvent() / (easeMs * 4000) - 0.125) + 0.125 - 2. * std::pow(((easeMs * 1000.) - (float)leftArrowButton.microsSinceEnd()) / (easeMs * 4000), 2.));
            debugGraphs[3].newGraphEntry(-3);
        }
        else {
            mrSquareStart = mrSquarePos;
            debugGraphs[3].newGraphEntry(0);
        }

        debugGraphs[1].newGraphEntry(mrSquarePos);


        cam.setCenter(cam.getCenter() + (coord(mrSquarePos, 0.) - cam.getCenter()) * 0.1);

        debugGraphs[2].newGraphEntry(cam.getCenter().x);

        // Graphics::drawLine(sf::Color::White, 4, rectWindow * UIVec(.5, .5), UIVec(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
        // (mouseWheelPosition);

        renderDebugOverlay(window);
        window.display();
    }

    return 0;
}