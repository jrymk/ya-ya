#ifndef _GRAPHICS_HPP_
#define _GRAPHICS_HPP_

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "debugger.hpp"

bool graphicsIsFullscreen = false;

struct UIVec { // pixel space
    float x = 0.;
    float y = 0.;
    UIVec() {};
    UIVec(float x, float y) {this->x = x; this->y = y;}
    UIVec operator+(const UIVec& r) {return {x + r.x, y + r.y};}
    UIVec operator-(const UIVec& r) {return {x - r.x, y - r.y};}
    UIVec operator*(const UIVec& r) {return {x * r.x, y * r.y};}
    sf::Vector2f getVec2f() {return sf::Vector2f(x, y);}
};

struct UIRect {
    UIVec pos;
    UIVec size;
    UIVec operator*(const UIVec& r) {return {pos + size * r};}
    UIRect() {}
    UIRect(float l, float t, float w, float h) {
        pos.x = l;
        pos.y = t;
        size.x = w;
        size.y = h;
    }
    UIRect(sf::FloatRect rect) {
        size = UIVec(rect.width, rect.height);
        pos = UIVec(rect.left, rect.top);
    }
};

class Graphics {
    inline static sf::RenderWindow* window;
    inline static unsigned int selectedFont;
    inline static std::vector<sf::Font> fonts;
    inline static std::vector<bool> isFontValid;

public:
    static void setRenderWindow(sf::RenderWindow& _window) {
        window = &_window;
    }

    static void loadFont(int idx, std::string path) {
        if (idx >= fonts.size()) {
            fonts.resize(idx + 1);
            isFontValid.resize(idx + 1);
        }
        if (!(isFontValid[idx] = fonts[idx].loadFromFile(path)))
            debug << "Failed to load font: " << path << "\n";
    }

    static void setFont(int idx) {
        if (idx >= isFontValid.size()) 
            debug << "Index out of range\n";
        else if (!isFontValid[idx])
            debug << "Font is invalid (didn't load or failed)\n";
        else
            selectedFont = idx;
    }

    static void drawText(const std::string& str, const sf::Color& color, int size, UIVec pos, float align = 0.) {
        sf::Text text;
        text.setPosition(0, 0);
        text.setFont(fonts[selectedFont]);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(color);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition((pos + UIVec(bounds.width * -align, -size)).getVec2f());

        if (debug_showWireframe) {
            // magenta rectangle: actual bounds
            sf::RectangleShape brect;
            brect.setPosition((pos + UIVec(bounds.width * -align, -size + bounds.top)).getVec2f());
            brect.setSize(sf::Vector2f(bounds.width, bounds.height));
            brect.setFillColor(sf::Color::Transparent);
            brect.setOutlineColor(sf::Color::Magenta);
            brect.setOutlineThickness(1);
            window->draw(brect);

            // cyan rectangle: baseline(input y coord) and size for reference
            sf::RectangleShape baseline;
            baseline.setPosition((pos + UIVec(bounds.width * -align, -size)).getVec2f());
            baseline.setSize(sf::Vector2f(bounds.width, size));
            baseline.setFillColor(sf::Color::Transparent);
            baseline.setOutlineColor(sf::Color::Cyan);
            baseline.setOutlineThickness(1);
            window->draw(baseline);
        }

        window->draw(text);
    }
};

#endif