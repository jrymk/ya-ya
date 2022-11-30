#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_
#include <chrono>
#include <queue>
#include <sstream>
#include <iomanip>
#include "graphics.hpp"

struct coord {
    double x = 0.;
    double y = 0.;
    coord() {};
    coord(double x, double y) {this->x = x; this->y = y;}
    coord operator+(const coord& r) {return {x + r.x, y + r.y};}
    coord operator-(const coord& r) {return {x - r.x, y - r.y};}
    coord operator*(const coord& r) {return {x * r.x, y * r.y};}
    coord operator*(const double r) {return {x * r, y * r};}
    coord operator/(const double r) {return {x / r, y / r};}
    coord min(const coord& r) {return {std::min(x, r.x), std::min(y, r.y)};}
    coord max(const coord& r) {return {std::max(x, r.x), std::max(y, r.y)};}
    double len() {return std::sqrt( double(x * x + y * y) );}
    double len(const coord& r) {return std::sqrt( double((x - r.x) * (x - r.x) + (y - r.y) * (y - r.y)) );}
    double angle(const coord& r) {return std::acos(double(r.x - x) / len(r)) * ((r.y - y >= 0) ? double(1) : double(-1));}
};

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