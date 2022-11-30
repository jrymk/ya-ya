#ifndef _EVENTS_HPP_
#define _EVENTS_HPP_

#include <SFML/Graphics.hpp>
#include "debugger.hpp"
#include "graphics.hpp"
#include "timer.hpp"

int mouseWheelPosition = 0;

Timer rightArrowButton;
Timer leftArrowButton;
bool updatePhysics = false;
void handleEvents(sf::RenderWindow& window) {
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
            if (event.key.code == sf::Keyboard::F2) {
                for (int i = 0; i < debugGraphs.size(); i++)
                    debugGraphs[i].resetRange();
            }
            if (event.key.code == sf::Keyboard::F3) {
                debugStream.str("");
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
            if (event.key.code == sf::Keyboard::F1) {
                debug_showWireframe = false;
            }
            if (event.key.code == sf::Keyboard::Right) {
            }
            if (event.key.code == sf::Keyboard::Left) {
            }
        }
        if (event.type == sf::Event::MouseWheelMoved) {
            mouseWheelPosition += event.mouseWheel.delta;
        }
    }
}

#endif