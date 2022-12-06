#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_
#include <iostream>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <deque>
#include <chrono>

#define debug debugStream << "<" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "> "

extern std::stringstream debugStream;
extern bool fontLoaded;
extern sf::Font debuggerFont;
extern std::vector<DebugGraph> debugGraphs;

class DebugGraph {
    std::string name;
    float graphWidth;
    float graphHeight;
    int timeSpan = 1000;
    std::deque<std::pair<std::chrono::steady_clock::time_point, float>> graph;
    bool resetMinMax = true;
    float graphMax = .01;
    float graphMin = -.01;

public:
    DebugGraph(const std::string& name, float graphWidth, float graphHeight, int ms);

    void resetRange();

    void setTimeSpanMs(int ms);

    void newGraphEntry(float value);

    float drawGraph(sf::RenderWindow& window, float y);
};


void renderDebugOverlay(sf::RenderWindow& window);

#endif