#include "debugger.h"

std::stringstream debugStream;
bool fontLoaded = false;
sf::Font debuggerFont;
std::vector<DebugGraph> debugGraphs;

DebugGraph::DebugGraph(const std::string& name, float graphWidth, float graphHeight, int ms) {
    this->name = name;
    this->graphWidth = graphWidth;
    this->graphHeight = graphHeight;
    this->timeSpan = ms;
}

void DebugGraph::resetRange() {
    resetMinMax = true;
}

void DebugGraph::setTimeSpanMs(int ms) {
    timeSpan = ms;
}

void DebugGraph::newGraphEntry(float value) {
    graph.push_back({std::chrono::steady_clock::now(), value});
    if (resetMinMax) {
        graphMax = graphMin = value;
        resetMinMax = false;
    }
    graphMax = std::max(graphMax, value);
    graphMin = std::min(graphMin, value);
}

float DebugGraph::drawGraph(sf::RenderWindow& window, float y) {
    sf::RectangleShape graphFrame;
    graphFrame.setPosition(window.getView().getSize().x - graphWidth - 10, y);
    graphFrame.setSize(sf::Vector2f(graphWidth, graphHeight));
    graphFrame.setFillColor(sf::Color(0, 0, 0, 100));
    graphFrame.setOutlineColor(sf::Color::Yellow);
    graphFrame.setOutlineThickness(-2); // positive: grow inwards
    window.draw(graphFrame);
    
    while (!graph.empty() && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - graph.front().first).count() >= timeSpan)
        graph.pop_front();

    for (int i = 0; i < graph.size(); i++) {
        int msNow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - graph[i].first).count();
        float val = graph[i].second;
        int msWidth = (i == 0 ? (timeSpan - msNow) : std::chrono::duration_cast<std::chrono::milliseconds>(graph[i].first - graph[i - 1].first).count());

        sf::RectangleShape rect;
        float right = window.getView().getSize().x - 10 - double(msNow) / timeSpan * graphWidth;
        float width = std::round((double)msWidth / timeSpan * graphWidth + 0.5);
        float height = std::max(std::min(graphHeight * (val - graphMin) / (graphMax - graphMin), graphHeight), (float)0);
        rect.setPosition(right - width, y + graphHeight - height);
        rect.setSize(sf::Vector2f(width, height));
        rect.setFillColor(sf::Color(255, 255, 255, 200));
        window.draw(rect);
    }

    graphFrame.setFillColor(sf::Color::Transparent);
    window.draw(graphFrame);

    sf::Text text;
    text.setPosition(0, 0);
    text.setFont(debuggerFont);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Yellow);
    text.setString(name);
    text.setPosition(window.getView().getSize().x - 10 - graphWidth, y - 18);
    window.draw(text);
    text.setFillColor(sf::Color::Cyan);
    std::stringstream ss;
    ss << graphMax;
    text.setString(ss.str());
    text.setPosition(window.getView().getSize().x - 7 - graphWidth, y + 2);
    window.draw(text);
    ss.str("");
    ss << graphMin;
    text.setString(ss.str());
    text.setPosition(window.getView().getSize().x - 7 - graphWidth, y + graphHeight - 22);
    window.draw(text);
    if (!graph.empty()) {
        ss.str("");
        ss << graph.back().second;
        text.setString(ss.str());
        text.setPosition(window.getView().getSize().x - 100, y - 18);
        window.draw(text);
    }
    return y + graphHeight + 25;
}

void renderDebugOverlay(sf::RenderWindow& window) {
    if (!fontLoaded) {
        if (!debuggerFont.loadFromFile("CascadiaCode.ttf")) {
            std::cerr << "Failed to load debugger font!\n";
            exit(-2);
        }
        fontLoaded = true;
    }
    sf::Text text;
    text.setPosition(0, 0);
    text.setFont(debuggerFont);
    text.setString(debugStream.str());
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::Red);
    sf::FloatRect bounds = text.getGlobalBounds();
    text.setPosition(6, window.getView().getSize().y - bounds.height + bounds.top - 20);
    text.setOutlineColor(sf::Color(255, 255, 255, 150));
    text.setOutlineThickness(2.);
    window.draw(text);

    sf::Text coord;
    coord.setPosition(0, 0);
    coord.setFont(debuggerFont);
    std::stringstream ss;
    ss << "size: (" << window.getView().getSize().x << ", " << window.getView().getSize().y << ")  cursor: (" << sf::Mouse::getPosition(window).x << ", " << sf::Mouse::getPosition(window).y << ")";
    coord.setString(ss.str());
    coord.setCharacterSize(16);
    coord.setFillColor(sf::Color::Green);
    sf::FloatRect boundsCoord = coord.getGlobalBounds();
    coord.setPosition(6, window.getView().getSize().y - boundsCoord.height + boundsCoord.top - 12);
    coord.setOutlineColor(sf::Color::Black);
    coord.setOutlineThickness(2.);
    window.draw(coord);

    // float y = 25;
    // for (int i = 0; i < debugGraphs.size(); i++) {
    //     y = debugGraphs[i].drawGraph(window, y);
    // }
}
