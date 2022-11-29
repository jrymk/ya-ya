#include "utilities.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "graphics.hpp"
#include "debugger.hpp"

/**
 * Key definitions
 * F1: show wireframe
 * 
 */

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window;
    window.create(sf::VideoMode(1600, 900), L"Ya-Ya!", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    
    Graphics::setRenderWindow(window);
    Graphics::loadFont(0, "yourStar.ttf");
    Graphics::loadFont(1, "CascadiaCode.ttf");
    FramerateCounter fc;

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
                    debug_showWireframe = true;
                }
                if (event.key.code == sf::Keyboard::F11) {
                    if (!graphicsIsFullscreen) {
                        graphicsIsFullscreen = true;
                        window.close();
                        window.create(sf::VideoMode::getFullscreenModes()[0], L"Ya-Ya!", sf::Style::Fullscreen, settings);
                        window.setVerticalSyncEnabled(true);
                    }
                    else {
                        graphicsIsFullscreen = false;
                        window.close();
                        window.create(sf::VideoMode(1600, 900), L"Ya-Ya!", sf::Style::Default, settings);
                        window.setVerticalSyncEnabled(true);
                    }
                    debug << "Toggled fullscreen: " << graphicsIsFullscreen << "\n";
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::F1) {
                    debug_showWireframe = false;
                }
            }
        }
        window.clear();

        UIRect rectWindow(sf::FloatRect(0., 0., window.getView().getSize().x, window.getView().getSize().y));

        Graphics::setFont(1);
        Graphics::drawText(toString(fc.getFramerate()) + "", sf::Color::White, 24, rectWindow * UIVec(.0, .0) + UIVec(10, 30), 0.);
        Graphics::setFont(0);
        Graphics::drawText("The quick 123 gg", sf::Color::White, 48, rectWindow * UIVec(.5, .5) + UIVec(0, 0), 0.5);

        Graphics::drawLine(sf::Color::White, 4, rectWindow * UIVec(.5, .5), UIVec(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));

        renderDebugOutput(window);
        window.display();
    }

    return 0;
}