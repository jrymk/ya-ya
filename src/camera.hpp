#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "utilities.hpp"
#include "graphics.hpp"

/// @brief handles mapping between different coordinate spaces
class Camera {
    UIRect viewport;
    coord center;
    double zoom; // the span of the camera, in sqrt(w*h) coord space

public:
    Camera() {
        center.x = 0.;
        center.y = 0.;
        zoom = 100.;
    }

    void setViewport(UIRect vp) { // set the viewport for the camera
        viewport = vp;
    }

    void setCenter(coord c) {center = c;}
    coord getCenter() {return center;}
    void setZoom(double z) {zoom = z;}
    double getZoom() {return zoom;}

    UIVec getScreenPos(coord loc) {
        double zoomRatio = std::sqrt(viewport.size.x * viewport.size.y) / zoom;
        return viewport.pos + viewport.size * UIVec(.5, .5) + 
            UIVec((loc.x - center.x) * zoomRatio, 
                  (center.y - loc.y) * zoomRatio);
    }

    coord getCoord(UIVec pos) {
        double zoomRatio = std::sqrt(viewport.size.x * viewport.size.y) / zoom;
        UIVec delta = (pos - (viewport.pos + viewport.size * UIVec(.5, .5))) / zoomRatio;
        return center + coord(delta.x, -delta.y);
    }
    
    void printCameraInfo() {
        Graphics::setFont(1);
        coord c = getCoord(UIVec(sf::Mouse::getPosition(*Graphics::getRenderWindow()).x, sf::Mouse::getPosition(*Graphics::getRenderWindow()).y));
        Graphics::drawText("(" + toString(c.x, 3) + ", " + toString(c.y, 3) + ")", sf::Color::Green, 16, viewport.pos + viewport.size - UIVec(5, 10), 1.);
    }
};

#endif