#pragma once
#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <SFML/Graphics.hpp>
#include "debugger.h"
#include "graphics.h"
#include "timer.h"

int mouseWheelPosition = 0;

Timer rightArrowButton;
Timer leftArrowButton;
bool updatePhysics = false;
void handleEvents(sf::RenderWindow& window);

#endif