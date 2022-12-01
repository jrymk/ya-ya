#ifndef _GRAPHICS_HPP_
#define _GRAPHICS_HPP_

#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "debugger.hpp"
#define PI 3.14159265359

bool graphicsIsFullscreen = false;

struct UIVec { // pixel space
    float x = 0.;
    float y = 0.;
    UIVec() {};
    UIVec(float x, float y) {this->x = x; this->y = y;}
    UIVec(sf::Vector2f v2) {this->x = v2.x; this->y = v2.y;}
    UIVec operator+(const UIVec& r) {return {x + r.x, y + r.y};}
    UIVec operator-(const UIVec& r) {return {x - r.x, y - r.y};}
    UIVec operator*(const UIVec& r) {return {x * r.x, y * r.y};}
    UIVec operator*(const float r) {return {x * r, y * r};}
    UIVec operator/(const float r) {return {x / r, y / r};}
    sf::Vector2f getVec2f() {return sf::Vector2f(x, y);}
    UIVec min(const UIVec& r) {return {std::min(x, r.x), std::min(y, r.y)};}
    UIVec max(const UIVec& r) {return {std::max(x, r.x), std::max(y, r.y)};}
    float len(const UIVec& r) {return std::sqrt( float((x - r.x) * (x - r.x) + (y - r.y) * (y - r.y)) );}
    float angle(const UIVec& r) {return std::acos(float(r.x - x) / len(r)) * ((r.y - y >= 0) ? float(1) : float(-1));}
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
    static const sf::RenderWindow* getRenderWindow() {
        return window;
    }

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

    static void fillRect(const sf::Color& fillColor, UIVec pos1, UIVec pos2) {
        sf::RectangleShape rect;
        rect.setPosition(pos1.min(pos2).getVec2f());
        rect.setSize(((pos1.max(pos2)) - (pos1.min(pos2))).getVec2f());
        rect.setFillColor(fillColor);
        window->draw(rect);
    }

    static void drawRect(const sf::Color& strokeColor, float strokeWidth, UIVec pos1, UIVec pos2) {
        sf::RectangleShape rect;
        rect.setPosition(pos1.min(pos2).getVec2f());
        rect.setSize(((pos1.max(pos2)) - (pos1.min(pos2))).getVec2f());
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(strokeColor);
        rect.setOutlineThickness(-strokeWidth); // positive: grow inwards
        window->draw(rect);
    }

    static void drawLine(const sf::Color& strokeColor, float strokeWidth, UIVec pos1, UIVec pos2) {
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(pos1.len(pos2), strokeWidth));
        rect.setOrigin(pos1.len(pos2) / 2., strokeWidth / 2.);
        rect.setPosition(((pos1 + pos2) / 2.).getVec2f());
        rect.setRotation(pos1.angle(pos2) / 3.14159265359 * 180);
        rect.setFillColor(strokeColor);
        window->draw(rect);
    }

    static void drawText(const std::string& str, const sf::Color& fillColor, int size, UIVec pos, float align = 0.) {
        sf::Text text;
        text.setPosition(0, 0);
        text.setFont(fonts[selectedFont]);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(fillColor);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition((pos + UIVec(bounds.width * -align, -size)).getVec2f());

        if (debug_showWireframe) {
            // magenta rectangle: actual bounds
            drawRect(sf::Color::Magenta, 1, pos + UIVec(bounds.width * -align, -size + bounds.top), pos + UIVec(bounds.width * -align, -size + bounds.top) + UIVec(bounds.width, bounds.height));
            // cyan rectangle: baseline(input y coord) and size for reference
            drawRect(sf::Color::Cyan, 1, pos + UIVec(bounds.width * -align, -size), pos + UIVec(bounds.width * -align, -size) + UIVec(bounds.width, size));
        }

        window->draw(text);
    }

    static void drawTextBatch(sf::VertexArray& va, const std::string& str, const sf::Color& fillColor, int size, UIVec pos, float align = 0.) {
        sf::Text text;
        text.setPosition(0, 0);
        text.setFont(fonts[selectedFont]);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(fillColor);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition((pos + UIVec(bounds.width * -align, -size)).getVec2f());

        if (debug_showWireframe) {
            // magenta rectangle: actual bounds
            drawRect(sf::Color::Magenta, 1, pos + UIVec(bounds.width * -align, -size + bounds.top), pos + UIVec(bounds.width * -align, -size + bounds.top) + UIVec(bounds.width, bounds.height));
            // cyan rectangle: baseline(input y coord) and size for reference
            drawRect(sf::Color::Cyan, 1, pos + UIVec(bounds.width * -align, -size), pos + UIVec(bounds.width * -align, -size) + UIVec(bounds.width, size));
        }
        window->draw(text);
    }
};

#endif