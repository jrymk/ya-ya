#include "camera.h"

void Camera::tieRenderWindow(sf::RenderWindow& window) {
    renderWindow = &window;
}

void Camera::setViewport(UIRect vp) { // set the viewport for the camera
    viewport = vp;
    updateScale();
}

UIRect Camera::getViewport() {
    return viewport;
}

sf::Transform Camera::getTransform() {
    updateScale();
    sf::Transform transform(
            3.840, -1.280, 0.000,
        -0.905, -2.715, 0.000,
            0.000,  0.000, 1.000
    );
    transform.scale(sf::Vector2f(scale, scale));
    return transform;
}

UIVec Camera::getScreenPos(coord loc) {
    return viewport.pos + viewport.size * UIVec(.5, .5) + getTransform().transformPoint(sf::Vector2f(loc.x - center.x, loc.y - center.y));
}

coord Camera::getCoord(UIVec pos) {
    return center + coord(getTransform().getInverse().transformPoint((pos - (viewport.pos + viewport.size * UIVec(.5, .5))).getVec2f()));
}

coord Camera::getMouseCoord() {
    return center + coord(getTransform().getInverse().transformPoint((UIVec(sf::Mouse::getPosition(*renderWindow).x, sf::Mouse::getPosition(*renderWindow).y) - (viewport.pos + viewport.size * UIVec(.5, .5))).getVec2f()));
}

UIVec Camera::getMousePos() {
    return UIVec(sf::Mouse::getPosition(*renderWindow).x, sf::Mouse::getPosition(*renderWindow).y) - viewport.pos;
}

UIVec Camera::getAngleVectorUntransformed(float len, double angle) {
    return UIVec(sf::Vector2f(std::cos(angle), std::sin(angle))) * len;
}

UIVec Camera::getAngleVector(float len, double angle) {
    return UIVec(getTransform().transformPoint(sf::Vector2f(std::cos(angle), std::sin(angle)))) * len;
}

void Camera::printCameraInfo() {
    Graphics::setFont(1);
    Graphics::drawText("(" + toStr(getMouseCoord().x, 3) + ", " + toStr(getMouseCoord().y, 3) + ")", sf::Color::Green, 16, viewport.pos + viewport.size - UIVec(5, 10), 1.);
}