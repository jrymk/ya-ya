#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
#include "debugger.hpp"

class Timer {
    std::chrono::steady_clock::time_point eventStart;
    std::chrono::steady_clock::time_point lastUpdate;
    std::chrono::steady_clock::time_point eventEnd;

public:
    int updatesSinceStart;
    int updatesSinceEnd;
    bool isStarted;

    int getElapsedMicrosAndUpdate() {
        std::chrono::steady_clock::time_point temp = lastUpdate;
        lastUpdate = std::chrono::steady_clock::now();
        updatesSinceStart++;
        updatesSinceEnd++;
        // debug << updatesSinceStart << "\n";
        // debug << std::chrono::duration_cast<std::chrono::microseconds>(lastUpdate - temp).count() << "\n";
        return std::chrono::duration_cast<std::chrono::microseconds>(lastUpdate - temp).count();
    }

    int microsSinceStart() {
        return std::chrono::duration_cast<std::chrono::microseconds>(lastUpdate - eventStart).count();
    }

    int microsSinceEnd() {
        return std::chrono::duration_cast<std::chrono::microseconds>(lastUpdate - eventEnd).count();
    }

    int microsEvent() {
        return std::chrono::duration_cast<std::chrono::microseconds>(eventEnd - eventStart).count();
    }

    void startEvent() {
        eventStart = std::chrono::steady_clock::now();
        updatesSinceStart = 0;
        updatesSinceEnd = 0;
        isStarted = true;
    }

    void endEvent() {
        eventEnd = std::chrono::steady_clock::now();
        updatesSinceEnd = 0;
        isStarted = false;
    }
};

#endif