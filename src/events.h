#pragma once
#ifndef YAYA_EVENTS_H
#define YAYA_EVENTS_H

#include <SFML/Graphics.hpp>
#include "debugger.h"
#include "graphics.h"
#include "timer.h"

int mouseWheelPosition = 0;

Timer rightArrowButton;
Timer leftArrowButton;
bool updatePhysics = false;

void handleEvents(sf::RenderWindow &window);

#endif