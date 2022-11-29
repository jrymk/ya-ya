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
    debugGraphs.push_back(DebugGraph("mouse wheel", 200, 150, 2500));
    // debugGraphs.push_back(DebugGraph(200, 150));

    Camera cam;

    double mrSquarePos = 0;

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
        Graphics::drawRect(sf::Color::White, 2, cam.getScreenPos({mrSquarePos, 0.}), cam.getScreenPos({mrSquarePos + 5.,5.}));
        Graphics::drawText(toString(mrSquarePos, 3), sf::Color::White, 16, cam.getScreenPos({mrSquarePos + 5 ,5.}) + UIVec(0, -5.), 1.);
        
        // if (rightArrowButton.isStarted) {
        //     if (rightArrowButton.updatesSinceStart == 0)
        //         rightArrowButton.getElapsedMicrosAndUpdate(); // discard
        //     else {
        //         int elapsed = rightArrowButton.getElapsedMicrosAndUpdate();
        //         // debug << (double)rightArrowButton.getElapsedMicrosAndUpdate() / (double)100000. * 1. << "\n";
        //         mrSquarePos += (double)elapsed / (double)1000000. * std::min(200000, rightArrowButton.getTotalElapsedMicrosSinceStart()) / 10000.; // 1 unit per second

        //         debugGraphs[0].newGraphEntry((double)elapsed / (double)1000000. * std::min(200000, rightArrowButton.getTotalElapsedMicrosSinceStart()) / 10000.);
        //     }
        // }
        // else {
        //     if (rightArrowButton.updatesSinceEnd == 0)
        //         rightArrowButton.getElapsedMicrosAndUpdate(); // discard
        //     else if (rightArrowButton.getTotalElapsedMicrosSinceEnd() < 100000) {
        //         int elapsed = rightArrowButton.getElapsedMicrosAndUpdate();
        //         // debug << (double)rightArrowButton.getElapsedMicrosAndUpdate() / (double)100000. * 1. << "\n";
        //         mrSquarePos += (double)elapsed / (double)1000000. * std::min(200000, rightArrowButton.getTotalElapsedMicrosSinceStart()) / 10000.; // 1 unit per second

        //         debugGraphs[0].newGraphEntry((double)elapsed / (double)1000000. * std::min(200000, (100000 - rightArrowButton.getTotalElapsedMicrosSinceEnd())) / 10000.);
        //     }
        // }

        // Graphics::drawLine(sf::Color::White, 4, rectWindow * UIVec(.5, .5), UIVec(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
        debugGraphs[1].newGraphEntry(mouseWheelPosition);

        renderDebugOverlay(window);
        window.display();
    }

    return 0;
}