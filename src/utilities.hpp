#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_
#include <chrono>
#include <queue>
#include <sstream>
#include <iomanip>
#include "graphics.hpp"

struct coord {
    double x;
    double y;
};

std::string toString(int a) {
    std::stringstream ss;
    ss << a;
    return ss.str();
}

std::string toString(double a, const int digits = -1) {
    std::stringstream ss;
    if (digits >= 0)
        ss << std::setw(digits);
    ss << a;
    return ss.str();
}

class FramerateCounter {
    std::queue<std::chrono::steady_clock::time_point> frametimes;
public:
    /// @brief inserts a new frame (update frame counter) and returns framerate
    /// @return framerate
    int getFramerate() {
        frametimes.push(std::chrono::steady_clock::now());
        while (!frametimes.empty() && std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - frametimes.front()).count() >= 1000000)
            frametimes.pop();
        return frametimes.size();
    }
};

#endif