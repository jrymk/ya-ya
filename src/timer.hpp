#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
#include "debugger.hpp"

class Timer {
    std::chrono::steady_clock::time_point lastTick;
    int tickElapsed = 0;

public:
    int getTickElapsed() {
        return tickElapsed;
    }

    void tick() {
        std::chrono::steady_clock::time_point temp = lastTick;
        lastTick = std::chrono::steady_clock::now();
        tickElapsed = std::chrono::duration_cast<std::chrono::microseconds>(lastTick - temp).count();
    }
};

#endif