#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_
#include <chrono>
#include <queue>
#include <sstream>
#include <iomanip>
#include "graphics.hpp"

std::string toString(int a) {
    std::stringstream ss;
    ss << a;
    return ss.str();
}

std::string toString(double a, const int digits = -1) {
    std::stringstream ss;
    if (digits >= 0)
        ss << std::fixed << std::setw(8) << std::setprecision(digits);
    ss << a;
    return ss.str();
}

class FramerateCounter {
    std::queue<std::chrono::steady_clock::time_point> frametimes;
    std::chrono::steady_clock::time_point prevFrame;
    int frametime = 0;
public:
    /// @brief inserts a new frame (update frame counter) and returns framerate
    /// @return framerate
    int getFramerateAndUpdate() {
        frametime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - prevFrame).count();
        prevFrame = std::chrono::steady_clock::now();
        frametimes.push(std::chrono::steady_clock::now());
        while (!frametimes.empty() && std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - frametimes.front()).count() >= 1000000)
            frametimes.pop();
        return frametimes.size();
    }

    float getFrametimeMs() {
        return (float)frametime / 1000;
    }

    FramerateCounter() {
        prevFrame = std::chrono::steady_clock::now();
    }
};

#endif