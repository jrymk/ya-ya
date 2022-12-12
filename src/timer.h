#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_

#include <string>
#include <vector>
#include <chrono>

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
};

#endif