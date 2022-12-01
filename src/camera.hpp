#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "utilities.hpp"
#include "graphics.hpp"

/// @brief handles mapping between different coordinate spaces
class Camera {
    inline static UIRect viewport;
    inline static coord center;
    inline static double zoom = 10.; // the span of the camera, in sqrt(w*h) coord space

public:
    static void setViewport(UIRect vp) { // set the viewport for the camera
        viewport = vp;
    }

    static UIRect getViewport() {
        return viewport;
    }

    static void setCenter(coord c) {center = c;}
    static coord getCenter() {return center;}
    static void setZoom(double z) {zoom = z;}
    static double getZoom() {return zoom;}

    static UIVec getScreenPos(coord loc) {
        double zoomRatio = std::sqrt(viewport.size.x * viewport.size.y) / zoom;
        return viewport.pos + viewport.size * UIVec(.5, .5) + 
            UIVec(((loc.x - center.x) * 3.84 - (loc.y - center.y) * 1.28) * zoomRatio, 
                  (-(loc.x - center.x) * 0.905 - (loc.y - center.y) * 2.715) * zoomRatio);
        // return viewport.pos + viewport.size * UIVec(.5, .5) + 
        //     UIVec((loc.x - center.x) * zoomRatio, 
        //           (center.y - loc.y) * zoomRatio);
    }

    static coord getCoord(UIVec pos) {
        double zoomRatio = std::sqrt(viewport.size.x * viewport.size.y) / zoom;
        UIVec delta = (pos - (viewport.pos + viewport.size * UIVec(.5, .5))) / zoomRatio;
        return center + coord(delta.x, -delta.y);
    }
    
    static void printCameraInfo() {
        Graphics::setFont(1);
        coord c = getCoord(UIVec(sf::Mouse::getPosition(*Graphics::getRenderWindow()).x, sf::Mouse::getPosition(*Graphics::getRenderWindow()).y));
        Graphics::drawText("(" + toString(c.x, 3) + ", " + toString(c.y, 3) + ")", sf::Color::Green, 16, viewport.pos + viewport.size - UIVec(5, 10), 1.);
    }
};

#endif