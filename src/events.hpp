#ifndef _EVENTS_HPP_
#define _EVENTS_HPP_

#include <SFML/Graphics.hpp>
#include "debugger.hpp"
#include "graphics.hpp"
#include "timer.hpp"

int mouseWheelPosition = 0;

Timer rightArrowButton;
Timer leftArrowButton;
bool updatePhysics = false;
void handleEvents(sf::RenderWindow& window) {
    
}

#endif