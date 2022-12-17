#pragma once
#ifndef YAYA_CAMERA_H
#define YAYA_CAMERA_H

#include "utilities.h"
#include "graphics.h"
#include "saveUtilities.h"

struct coord {
    double x = 0.;
    double y = 0.;

    inline coord() {};

    inline coord(double x, double y) {
        this->x = x;
        this->y = y;
    }

    inline coord(sf::Vector2f v2) {
        this->x = v2.x;
        this->y = v2.y;
    }

    inline coord operator+(const coord &r) const { return {x + r.x, y + r.y}; }

    inline coord operator-(const coord &r) const { return {x - r.x, y - r.y}; }

    inline coord operator*(const coord &r) const { return {x * r.x, y * r.y}; }

    inline coord operator*(const double r) const { return {x * r, y * r}; }

    inline coord operator/(const double r) const { return {x / r, y / r}; }

    inline coord min(const coord &r) const { return {std::min(x, r.x), std::min(y, r.y)}; }

    inline coord max(const coord &r) const { return {std::max(x, r.x), std::max(y, r.y)}; }

    inline double len() const { return std::sqrt(double(x * x + y * y)); }

    inline double len(const coord &r) const { return std::sqrt(double((x - r.x) * (x - r.x) + (y - r.y) * (y - r.y))); }

    inline coord unit() const { return (*this) / (*this).len(); }

    inline double angle(const coord &r) { return std::acos(double(r.x - x) / len(r)) * ((r.y - y >= 0) ? double(1) : double(-1)); }

    inline static coord getRandCoord() { return {.01 * (getRand() - .5), .01 * (getRand() - .5)}; }

    inline static coord getAngleVec(double len, double angle) {
        return coord(std::cos(angle), std::sin(angle)) * len;
    }

    inline UIVec getUIVec() { return UIVec(x, y); }

    inline constexpr static auto properties = std::make_tuple(
            SaveUtilities::property(&coord::x, "cd.x"),
            SaveUtilities::property(&coord::y, "cd.y")
    );
};

/// @brief handles mapping between different coordinate spaces
class Camera {
    inline static sf::RenderWindow* renderWindow;
    inline static UIRect viewport;
    inline static coord center;
    inline static double zoom = 15.; // the span of the camera, in sqrt(w*h) coord space
    inline static double scale = 1.;

    inline static void updateScale() { scale = std::sqrt(viewport.size.x * viewport.size.y) / zoom; }

public:
    static void tieRenderWindow(sf::RenderWindow &window);

    static void setViewport(UIRect vp);

    static UIRect getViewport();

    inline static void setCenter(coord c) { center = c; }

    inline static coord getCenter() { return center; }

    inline static void setZoom(double z) {
        zoom = z;
        updateScale();
    }

    inline static double getZoom() { return zoom; }

    inline static double getScale() {
        updateScale();
        return scale;
    }

    static sf::Transform getTransform();

    static UIVec getScreenPos(coord loc, double zPos = 0.);

    static coord getCoord(UIVec pos);

    static coord getMouseCoord();

    static UIVec getMousePos();

    static UIVec getAngleVectorUntransformed(float len, double angle);

    static UIVec getAngleVector(float len, double angle);

    static void printCameraInfo();
};

#endif