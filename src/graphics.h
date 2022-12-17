#pragma once
#ifndef YAYA_GRAPHICS_H
#define YAYA_GRAPHICS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "debugger.h"
#include "saveUtilities.h"

#define ZDEPTH_LAYER 0.0001 // on-screen y position causes +-0.005 zdepth offset
#define ZDEPTH_BG 0.
#define ZDEPTH_GROUND 0.1
#define ZDEPTH_GROUND_OVERLAY 0.2
#define ZDEPTH_ABOVE_GROUND 0.4

#define PI 3.14159265359

extern bool graphicsIsFullscreen;

struct UIVec { // pixel space
    float x = 0.;
    float y = 0.;

    inline UIVec() = default;

    inline UIVec(float x, float y) {
        this->x = x;
        this->y = y;
    }

    inline UIVec(sf::Vector2f v2) {
        this->x = v2.x;
        this->y = v2.y;
    }

    inline UIVec operator+(const UIVec r) const { return {x + r.x, y + r.y}; }

    inline UIVec operator-(const UIVec r) const { return {x - r.x, y - r.y}; }

    inline UIVec operator*(const UIVec r) const { return {x * r.x, y * r.y}; }

    inline UIVec operator*(const float r) const { return {x * r, y * r}; }

    inline UIVec operator/(const float r) const { return {x / r, y / r}; }

    inline sf::Vector2f getVec2f() const { return {x, y}; }

    inline UIVec min(const UIVec r) const { return {std::min(x, r.x), std::min(y, r.y)}; }

    inline UIVec max(const UIVec r) const { return {std::max(x, r.x), std::max(y, r.y)}; }

    inline float len() const { return std::sqrt(float(x * x + y * y)); }

    inline float len(const UIVec r) const { return std::sqrt(float((x - r.x) * (x - r.x) + (y - r.y) * (y - r.y))); }

    inline float angle(const UIVec r) { return std::acos(float(r.x - x) / len(r)) * ((r.y - y >= 0) ? float(1) : float(-1)); }

    constexpr static auto properties = std::make_tuple(
        SaveUtilities::property(&UIVec::x, "Uv.xx"),
        SaveUtilities::property(&UIVec::y, "Uv.yy")
    );
};

struct UIRect {
    UIVec pos;
    UIVec size;

    inline UIRect() = default;

    inline UIVec operator*(const UIVec r) const { return {pos + size * r}; }

    inline UIRect(float l, float t, float w, float h) {
        pos.x = l;
        pos.y = t;
        size.x = w;
        size.y = h;
    }

    inline UIRect(UIVec tl, float w, float h) {
        pos.x = tl.x;
        pos.y = tl.y;
        size.x = w;
        size.y = h;
    }

    inline UIRect(UIVec tl, UIVec wh) {
        pos.x = tl.x;
        pos.y = tl.y;
        size.x = wh.x;
        size.y = wh.y;
    }

    inline UIRect(sf::FloatRect rect) {
        size = UIVec(rect.width, rect.height);
        pos = UIVec(rect.left, rect.top);
    }
};

class Graphics {
public:
    struct Quad {
        float zDepth = 1.0;
        float zDepthOffset = 0.;
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
        float zPosScale = 1.;

        inline Quad(float zDepth, UIVec v0, sf::Vector2f t0, UIVec v1, sf::Vector2f t1, UIVec v2, sf::Vector2f t2, UIVec v3, sf::Vector2f t3)
                : zDepth(zDepth), v0(v0), t0(t0), v1(v1), t1(t1), v2(v2), t2(t2), v3(v3), t3(t3) {}

        inline Quad(float zDepth, float zDepthOffset, UIVec v0, sf::Vector2f t0, UIVec v1, sf::Vector2f t1, UIVec v2, sf::Vector2f t2, UIVec v3,
                    sf::Vector2f t3) // for egg carton :angry:
                : zDepth(zDepth), v0(v0), t0(t0), v1(v1), t1(t1), v2(v2), t2(t2), v3(v3), t3(t3), zDepthOffset(zDepthOffset) {}

        inline Quad(float zDepth, UIVec v0, sf::Vector2f t0, UIVec v1, sf::Vector2f t1, UIVec v2, sf::Vector2f t2, UIVec v3, sf::Vector2f t3, const sf::Color &gc)
                : zDepth(zDepth), v0(v0), t0(t0), c0(gc), v1(v1), t1(t1), c1(gc), v2(v2), t2(t2), c2(gc), v3(v3), t3(t3), c3(gc) {}

        inline Quad(float zDepth, UIVec v0, sf::Vector2f t0, const sf::Color &c0, UIVec v1, sf::Vector2f t1, const sf::Color &c1, UIVec v2, sf::Vector2f t2, const sf::Color &c2,
                    UIVec v3, sf::Vector2f t3, const sf::Color &c3)
                : zDepth(zDepth), v0(v0), t0(t0), c0(c0), v1(v1), t1(t1), c1(c1), v2(v2), t2(t2), c2(c2), v3(v3), t3(t3), c3(c3) {}

        inline Quad(float zDepth, UIVec v0, sf::Vector2f t0, UIVec v1, sf::Vector2f t1, UIVec v2, sf::Vector2f t2, UIVec v3, sf::Vector2f t3, float zPos)
                : zDepth(zDepth), v0(v0), t0(t0), v1(v1), t1(t1), v2(v2), t2(t2), v3(v3), t3(t3), zPosScale(zPos) {}

        inline Quad(float zDepth, UIVec v0, sf::Vector2f t0, UIVec v1, sf::Vector2f t1, UIVec v2, sf::Vector2f t2, UIVec v3, sf::Vector2f t3, const sf::Color &gc, float zPos)
                : zDepth(zDepth), v0(v0), t0(t0), c0(gc), v1(v1), t1(t1), c1(gc), v2(v2), t2(t2), c2(gc), v3(v3), t3(t3), c3(gc), zPosScale(zPos) {}

        inline Quad(float zDepth, UIVec v0, sf::Vector2f t0, const sf::Color &c0, UIVec v1, sf::Vector2f t1, const sf::Color &c1, UIVec v2, sf::Vector2f t2, const sf::Color &c2,
                    UIVec v3, sf::Vector2f t3, const sf::Color &c3, float zPos)
                : zDepth(zDepth), v0(v0), t0(t0), c0(c0), v1(v1), t1(t1), c1(c1), v2(v2), t2(t2), c2(c2), v3(v3), t3(t3), c3(c3), zPosScale(zPos) {}

        inline bool operator<(const Quad &rhs) const {
            return (zDepth < rhs.zDepth);
        }
    };

    struct Image {
        sf::Vector2f texturePos;
        sf::Vector2f textureSize;
        float defaultScale = 1.;

        inline Image(sf::Vector2f pos, sf::Vector2f size) : texturePos(pos), textureSize(size) {}

        inline Image(sf::Vector2f pos, sf::Vector2f size, float defaultScale) : texturePos(pos), textureSize(size), defaultScale(defaultScale) {}
    };

private:
    inline static sf::RenderWindow* window;
    inline static unsigned int selectedFont;
    inline static std::vector<sf::Font> fonts;
    inline static std::vector<bool> isFontValid;
    inline static std::vector<Quad> quadsArray;
    inline static sf::VertexArray vertexArray;
    inline static sf::VertexArray wireframeVertexArray;
    inline static sf::VertexArray userWireframeVertexArray;

public:
    inline static bool showWireframe = false;
    inline static bool debugOutOfSight = false;
    inline static int outOfSightQuads = 0;

    static sf::RenderWindow &getRenderWindow() { return *window; }

    static void setRenderWindow(sf::RenderWindow &_window) { window = &_window; }

    static void loadFont(int idx, std::string path);

    static void setFont(int idx);

    static void fillRect(const sf::Color &fillColor, UIVec pos1, UIVec pos2);

    static void drawRect(const sf::Color &strokeColor, float strokeWidth, UIVec pos1, UIVec pos2);

    static void drawLine(const sf::Color &strokeColor, float strokeWidth, UIVec pos1, UIVec pos2);

    static void drawText(const std::string &str, const sf::Color &fillColor, int size, UIVec pos, float align = 0., const sf::Color &outlineColor = sf::Color::Black,
                         float outlineStrokeWidth = 0.);

    static void drawText(const std::wstring &str, const sf::Color &fillColor, int size, UIVec pos, float align = 0., const sf::Color &outlineColor = sf::Color::Black,
                         float outlineStrokeWidth = 0.);

    static void drawTextBatch(sf::VertexArray &va, const std::string &str, const sf::Color &fillColor, int size, UIVec pos, float align = 0.);

    static void drawImage(const Image &image, float zDepth, UIVec pos, UIVec align, float scale = 1., sf::Color color = sf::Color::White);

    static void appendWireframe(UIVec v0, UIVec v1, UIVec v2, UIVec v3, const sf::Color &color = sf::Color::Black, const sf::Color &bgcolor = sf::Color::White);

    static void clearQuadsArray();

    static void insertQuad(const Graphics::Quad &quad);

    static void insertUserWireframe(UIVec v0, UIVec v1, UIVec v2, UIVec v3, const sf::Color &color = sf::Color::Black, const sf::Color &bgcolor = sf::Color::White);

    static void renderQuads(sf::RenderWindow &window, sf::Texture &texture, UIRect viewport);

    static int getQuadCount();

    static void resizeView(unsigned int width, unsigned int height);

    static sf::ContextSettings getContextSettings();

    static void setupWindow();

    static void createWindow(bool fullscreen);
};

#endif