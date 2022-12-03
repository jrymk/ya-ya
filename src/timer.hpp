#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
#include "debugger.hpp"

class Timer {
    inline static std::chrono::steady_clock::time_point globalStart = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point tp;

public:
    Timer() { tp = std::chrono::steady_clock::now(); }
    Timer(const Timer& timer) { tp = timer.tp; }
    Timer(std::chrono::steady_clock::time_point tp): tp(tp) {}

    static Timer getNow() {
        return Timer(std::chrono::steady_clock::now());
    }

    static Timer getGlobalStart() {
        return globalStart;
    }

    double elapsed(Timer start = std::chrono::steady_clock::now()) {
        return std::chrono::duration<double>(start.tp - tp).count();
    }

    Timer operator+(double seconds) const {
        return Timer(tp + std::chrono::steady_clock::duration(std::chrono::microseconds((int)(seconds * 1000000))));
    }

    void increment(double seconds) {
        tp += std::chrono::steady_clock::duration(std::chrono::microseconds((int)(seconds * 1000000)));
    }

    bool operator<(const Timer& rhs) const {
        return (tp < rhs.tp);
    }
};

#endif