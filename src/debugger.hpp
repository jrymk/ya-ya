#ifndef _DEBUGGER_HPP_
#define _DEBUGGER_HPP_
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#define debug debugStream << "<" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "> "

std::stringstream debugStream;
bool fontLoaded = false;
sf::Font debuggerFont;

bool debug_showWireframe = false;

void renderDebugOutput(sf::RenderWindow& window) {
    if (!fontLoaded) {
        if (!debuggerFont.loadFromFile("CascadiaCode.ttf")) {
            std::cerr << "Failed to load debugger font!\n";
            exit(-2);
        }
        fontLoaded = true;
    }
    sf::Text text;
    text.setPosition(0, 0);
    text.setFont(debuggerFont);
    text.setString(debugStream.str());
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Red);
    sf::FloatRect bounds = text.getGlobalBounds();
    text.setPosition(6, window.getView().getSize().y - bounds.height + bounds.top - 20);
    window.draw(text);

    sf::Text coord;
    coord.setPosition(0, 0);
    coord.setFont(debuggerFont);
    std::stringstream ss;
    ss << "size: (" << window.getView().getSize().x << ", " << window.getView().getSize().y << ")  cursor: (" << sf::Mouse::getPosition(window).x << ", " << sf::Mouse::getPosition(window).y << ")";
    coord.setString(ss.str());
    coord.setCharacterSize(16);
    coord.setFillColor(sf::Color::Green);
    sf::FloatRect boundsCoord = coord.getGlobalBounds();
    coord.setPosition(6, window.getView().getSize().y - boundsCoord.height + boundsCoord.top - 12);
    window.draw(coord);
}

#endif