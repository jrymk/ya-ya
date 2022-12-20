#pragma once
#ifndef YAYA_UTILITIES_H
#define YAYA_UTILITIES_H

#include <string>
#include <queue>
#include <chrono>
#include <random>

#define prm(a) " " + toStr(a)

std::string toStr(int a);

std::string toStr(double a, const int digits = -1);

std::wstring toWStr(int a);

std::pair<std::string, std::string> splitId(std::string id);

class FramerateCounter {
    std::queue<std::chrono::steady_clock::time_point> frametimes;
    std::chrono::steady_clock::time_point prevFrame;
    int frametime = 0;
public:
    /// @brief inserts a new frame (update frame counter) and returns framerate
    /// @return framerate
    int getFramerateAndUpdate();

    float getFrametimeMs();

    FramerateCounter();
};

extern std::mt19937 rng;

double cap(double d);

double getRand();

uint32_t getRandInt();

double subtractAngle(double l, double r);

std::string randomId();

std::string randomName();

#endif