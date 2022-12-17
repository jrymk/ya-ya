#pragma once
#ifndef YAYA_TIMER_H
#define YAYA_TIMER_H

#include <string>
#include <vector>
#include <chrono>
#include "saveUtilities.h"

#define TIME_SMALL_INC 1E-6

class Timer {
    inline static std::chrono::steady_clock::time_point globalStart = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point tp;

public:
    Timer();

    Timer(const Timer &timer);

    Timer(std::chrono::steady_clock::time_point tp);

    static Timer getNow();

    static Timer getGlobalStart();

    double elapsed(Timer start = std::chrono::steady_clock::now());

    Timer operator+(double seconds) const;

    void increment(double seconds);

    bool operator<(const Timer &rhs) const;

    double lastTimefromTP = 0.;  // for game load
    void saveTimer(Timer start);
    void loadTimer(Timer start);

    constexpr static auto properties = std::make_tuple(
        SaveUtilities::property(&Timer::lastTimefromTP, "Ti.ltt")
    );
};

#endif