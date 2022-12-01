#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "utilities.hpp"
#include "graphics.hpp"

struct coord {
    double x = 0.;
    double y = 0.;
    coord() {};
    coord(double x, double y) {this->x = x; this->y = y;}
    coord(sf::Vector2f v2) {this->x = v2.x; this->y = v2.y;}
    coord operator+(const coord& r) {return {x + r.x, y + r.y};}
    coord operator-(const coord& r) {return {x - r.x, y - r.y};}
    coord operator*(const coord& r) {return {x * r.x, y * r.y};}
    coord operator*(const double r) {return {x * r, y * r};}
    coord operator/(const double r) {return {x / r, y / r};}
    coord min(const coord& r) {return {std::min(x, r.x), std::min(y, r.y)};}
    coord max(const coord& r) {return {std::max(x, r.x), std::max(y, r.y)};}
    double len() {return std::sqrt( double(x * x + y * y) );}
    double len(const coord& r) {return std::sqrt( double((x - r.x) * (x - r.x) + (y - r.y) * (y - r.y)) );}
    double angle(const coord& r) {return std::acos(double(r.x - x) / len(r)) * ((r.y - y >= 0) ? double(1) : double(-1));}
};

/// @brief handles mapping between different coordinate spaces
class Camera {
    inline static UIRect viewport;
    inline static coord center;
    inline static double zoom = 32.; // the span of the camera, in sqrt(w*h) coord space
    inline static double scale = 1.;

    static void updateScale() { scale = std::sqrt(viewport.size.x * viewport.size.y) / zoom; }

public:
    static void setViewport(UIRect vp) { // set the viewport for the camera
        viewport = vp;
        updateScale();
    }

    static UIRect getViewport() {
        return viewport;
    }

    static void setCenter(coord c) { center = c; }
    static coord getCenter() { return center; }
    static void setZoom(double z) { zoom = z; updateScale(); }
    static double getZoom() { return zoom; }

    static sf::Transform getTransform() {
        updateScale();
        sf::Transform transform(
             3.840, -1.280, 0.000,
            -0.905, -2.715, 0.000,
             0.000,  0.000, 1.000
        );
        transform.scale(sf::Vector2f(scale, scale));
        return transform;
    }

    static UIVec getScreenPos(coord loc) {
        return viewport.pos + viewport.size * UIVec(.5, .5) + getTransform().transformPoint(sf::Vector2f(loc.x - center.x, loc.y - center.y));
    }

    static coord getCoord(UIVec pos) {
        return center + coord(getTransform().getInverse().transformPoint((pos - (viewport.pos + viewport.size * UIVec(.5, .5))).getVec2f()));
    }
    
    static void printCameraInfo() {
        Graphics::setFont(1);
        coord c = getCoord(UIVec(sf::Mouse::getPosition(*Graphics::getRenderWindow()).x, sf::Mouse::getPosition(*Graphics::getRenderWindow()).y));
        Graphics::drawText("(" + toString(c.x, 3) + ", " + toString(c.y, 3) + ")", sf::Color::Green, 16, viewport.pos + viewport.size - UIVec(5, 10), 1.);
    }
};

#endif