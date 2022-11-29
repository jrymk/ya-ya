#include <iostream>
#include <SFML/Graphics.hpp>
#include "utilities.hpp"
#include "camera.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), L"Ya-Ya!");
    window.setVerticalSyncEnabled(true); 
    
    sf::Font font;
    if (!font.loadFromFile("yourStar.ttf"))
        std::cerr << "load font failed\n";

    FramerateCounter fc;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        sf::Text text;

        text.setFont(font);
        text.setString(toString(fc.getFramerate()));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        window.draw(text);

        window.display();
    }

    return 0;
}