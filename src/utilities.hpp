#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_
#include <chrono>
#include <queue>
#include <sstream>
#include <iomanip>
#include <random>
#include <ctime>
#include <cstdlib>
#include "graphics.hpp"
#include "names.hpp"

#define prm(a) " " + toStr(a)

std::string toStr(int a) {
    std::stringstream ss;
    ss << a;
    return ss.str();
}

std::string toStr(double a, const int digits = -1) {
    std::stringstream ss;
    if (digits >= 0)
        ss << std::fixed << std::setw(8) << std::setprecision(digits);
    ss << a;
    return ss.str();
}

// std::string toStr(coord c) {
//     return toStr(c.x) + " " + toStr(c.y);
// }

std::pair<std::string, std::string> splitId(std::string id) {
    auto rbrckt = id.find('$');
    if (rbrckt == id.npos || id.length() < 2)
        return std::make_pair("", id);
    return std::make_pair(id.substr(0, rbrckt), id.substr(rbrckt + 1));
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

std::mt19937 rng(time(0));

double getRand() {
    return std::max(0., std::min(1., (double)rng() / rng.max()));
}

double subtractAngle(double l, double r) {
    return (((l - r) / 2. / PI) - int((l - r) / 2. / PI)) * 2. * PI;
    // return l - r + 6 * PI - int((l - r + 6 * PI) / 2. / PI) * 2. * PI;
}

std::string randomId() {
    constexpr const char* charset = "0123456789abcdef";
    std::string s;
    for (int i = 0; i < 8; i++)
        s += charset[int(getRand() * 65536) % 16];
    return s;
}

std::string randomName() {
    return namesList[int(getRand() * 18239.)];
}

#endif