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
    UIVec operator+(const UIVec r) {return {x + r.x, y + r.y};}
    UIVec operator-(const UIVec r) {return {x - r.x, y - r.y};}
    UIVec operator*(const UIVec r) {return {x * r.x, y * r.y};}
    UIVec operator*(const float r) {return {x * r, y * r};}
    UIVec operator/(const float r) {return {x / r, y / r};}
    sf::Vector2f getVec2f() {return sf::Vector2f(x, y);}
    UIVec min(const UIVec r) {return {std::min(x, r.x), std::min(y, r.y)};}
    UIVec max(const UIVec r) {return {std::max(x, r.x), std::max(y, r.y)};}
    float len(const UIVec r) {return std::sqrt( float((x - r.x) * (x - r.x) + (y - r.y) * (y - r.y)) );}
    float angle(const UIVec r) {return std::acos(float(r.x - x) / len(r)) * ((r.y - y >= 0) ? float(1) : float(-1));}
};

struct UIRect {
    UIVec pos;
    UIVec size;
    UIVec operator*(const UIVec r) {return {pos + size * r};}
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
public:
    struct Quad {
        float zDepth = 1.0;
        UIVec v0;
        UIVec v1;
        UIVec v2;
        UIVec v3;
        sf::Vector2f t0;
        sf::Vector2f t1;
        sf::Vector2f t2;
        sf::Vector2f t3;
        sf::Color c0 = sf::Color::White;
        sf::Color c1 = sf::Color::White;
        sf::Color c2 = sf::Color::White;
        sf::Color c3 = sf::Color::White;

        Quad() {}
        Quad(float zDepth, UIVec v0, sf::Vector2f t0, UIVec v1, sf::Vector2f t1, UIVec v2, sf::Vector2f t2, UIVec v3, sf::Vector2f t3)
            : zDepth(zDepth), v0(v0), t0(t0), v1(v1), t1(t1), v2(v2), t2(t2), v3(v3), t3(t3) {}
        Quad(float zDepth,UIVec v0, sf::Vector2f t0, UIVec v1, sf::Vector2f t1, UIVec v2, sf::Vector2f t2, UIVec v3, sf::Vector2f t3, const sf::Color& gc)
            : zDepth(zDepth), v0(v0), t0(t0), c0(gc), v1(v1), t1(t1), c1(gc), v2(v2), t2(t2), c2(c2), v3(v3), t3(t3), c3(gc) {}
        Quad(float zDepth,UIVec v0, sf::Vector2f t0, const sf::Color& c0, UIVec v1, sf::Vector2f t1, const sf::Color& c1, UIVec v2, sf::Vector2f t2, const sf::Color& c2, UIVec v3, sf::Vector2f t3, const sf::Color& c3)
            : zDepth(zDepth), v0(v0), t0(t0), c0(c0), v1(v1), t1(t1), c1(c1), v2(v2), t2(t2), c2(c2), v3(v3), t3(t3), c3(c3) {}

        bool operator<(const Quad& rhs) const {
            return (zDepth < rhs.zDepth);
        }
    };

private:
    inline static sf::RenderWindow* window;
    inline static unsigned int selectedFont;
    inline static std::vector<sf::Font> fonts;
    inline static std::vector<bool> isFontValid;
    inline static std::vector<Quad> quadsArray; 
    inline static sf::VertexArray vertexArray;
    inline static sf::VertexArray wireframeVertexArray;

public:
    inline static bool showWireframe = false;
    inline static bool debugOutOfSight = false;
    inline static int outOfSightQuads = 0;

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

    static void drawText(const std::string& str, const sf::Color& fillColor, int size, UIVec pos, float align = 0., const sf::Color& outlineColor = sf::Color::Black, float outlineStrokeWidth = 0.) {
        sf::Text text;
        text.setPosition(0, 0);
        text.setFont(fonts[selectedFont]);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(fillColor);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition((pos + UIVec(bounds.width * -align, -size)).getVec2f());
        text.setOutlineColor(outlineColor);
        text.setOutlineThickness(outlineStrokeWidth);
        window->draw(text);

        if (showWireframe) {
            // magenta rectangle: actual bounds
            drawRect(sf::Color::Magenta, 1, pos + UIVec(bounds.width * -align, -size + bounds.top), pos + UIVec(bounds.width * -align, -size + bounds.top) + UIVec(bounds.width, bounds.height));
            // cyan rectangle: baseline(input y coord) and size for reference
            drawRect(sf::Color::Cyan, 1, pos + UIVec(bounds.width * -align, -size), pos + UIVec(bounds.width * -align, -size) + UIVec(bounds.width, size));
        }
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
        window->draw(text);

        if (showWireframe) {
            // magenta rectangle: actual bounds
            drawRect(sf::Color::Magenta, 1, pos + UIVec(bounds.width * -align, -size + bounds.top), pos + UIVec(bounds.width * -align, -size + bounds.top) + UIVec(bounds.width, bounds.height));
            // cyan rectangle: baseline(input y coord) and size for reference
            drawRect(sf::Color::Cyan, 1, pos + UIVec(bounds.width * -align, -size), pos + UIVec(bounds.width * -align, -size) + UIVec(bounds.width, size));
        }
    }

private:
    static void appendWireframe(UIVec v0, UIVec v1, UIVec v2, UIVec v3, const sf::Color& color = sf::Color::White) {
        wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., 1.), sf::Color::Black));

        wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(1., -1.), sf::Color::Black));

        wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., 1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., 1.), sf::Color::Black));

        wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f() + sf::Vector2f(-1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f() + sf::Vector2f(-1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f() + sf::Vector2f(-1., -1.), sf::Color::Black));
        wireframeVertexArray.append(sf::Vertex(v0.getVec2f() + sf::Vector2f(-1., -1.), sf::Color::Black));
        
        wireframeVertexArray.append(sf::Vertex(v0.getVec2f(), color));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f(), color));
        wireframeVertexArray.append(sf::Vertex(v1.getVec2f(), color));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f(), color));
        wireframeVertexArray.append(sf::Vertex(v2.getVec2f(), color));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f(), color));
        wireframeVertexArray.append(sf::Vertex(v3.getVec2f(), color));
        wireframeVertexArray.append(sf::Vertex(v0.getVec2f(), color));
    }

public:
    static void clearQuadsArray() {
        quadsArray.clear();
    }

    static void insertQuad(const Graphics::Quad& quad) {
        quadsArray.push_back(quad);
    }

    static void renderQuads(sf::RenderWindow& window, sf::Texture& texture) {
        vertexArray.clear();
        vertexArray.setPrimitiveType(sf::PrimitiveType::Quads);
        wireframeVertexArray.clear();
        wireframeVertexArray.setPrimitiveType(sf::PrimitiveType::Lines);

        std::sort(quadsArray.begin(), quadsArray.end());
        outOfSightQuads = 0;

        for (auto& quad : quadsArray) {
            if ((quad.v0.x < 0 || quad.v0.x > window.getView().getSize().x ||
                quad.v0.y < 0 || quad.v0.y > window.getView().getSize().y) &&
                (quad.v1.x < 0 || quad.v1.x > window.getView().getSize().x ||
                quad.v1.y < 0 || quad.v1.y > window.getView().getSize().y) &&
                (quad.v2.x < 0 || quad.v2.x > window.getView().getSize().x ||
                quad.v2.y < 0 || quad.v2.y > window.getView().getSize().y) &&
                (quad.v3.x < 0 || quad.v3.x > window.getView().getSize().x ||
                quad.v3.y < 0 || quad.v3.y > window.getView().getSize().y)) {
                    
                if (debugOutOfSight) {
                    UIVec center = (quad.v0 + quad.v1 + quad.v2 + quad.v3) / 4.;
                    center.x = std::min(std::max(center.x, (float)5.), window.getView().getSize().x - (float)5.);
                    center.y = std::min(std::max(center.y, (float)5.), window.getView().getSize().y - (float)5.);
                    appendWireframe(center + UIVec(3., 0.),
                                    center + UIVec(0., 3.),
                                    center + UIVec(-3., 0.),
                                    center + UIVec(0., -3.),
                                    sf::Color::Yellow);
                }
                outOfSightQuads++;
            }
            else {
                vertexArray.append(sf::Vertex(quad.v0.getVec2f(), quad.c0, quad.t0));
                vertexArray.append(sf::Vertex(quad.v1.getVec2f(), quad.c1, quad.t1));
                vertexArray.append(sf::Vertex(quad.v2.getVec2f(), quad.c2, quad.t2));
                vertexArray.append(sf::Vertex(quad.v3.getVec2f(), quad.c3, quad.t3));
            }

            if (showWireframe)
                appendWireframe(quad.v0, quad.v1, quad.v2, quad.v3);
        }

        window.draw(vertexArray, &texture);
        if (showWireframe)
            window.draw(wireframeVertexArray);
    }

    static int getQuadCount() {
        return vertexArray.getVertexCount() / 4;
    }
};

#endif