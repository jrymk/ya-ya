#include "timer.h"
#include "debugger.h"
#include "graphics.h"
#include "utilities.h"
#include <sstream>
#include <iomanip>

Timer::Timer() { tp = std::chrono::steady_clock::now(); }

Timer::Timer(const Timer &timer) { tp = timer.tp; }

Timer::Timer(std::chrono::steady_clock::time_point tp) : tp(tp) {}

Timer Timer::getNow() {
    return Timer(std::chrono::steady_clock::now());
}

Timer Timer::getGlobalStart() {
    return globalStart;
}

double Timer::elapsed(Timer start) {
    return std::chrono::duration<double>(start.tp - tp).count();
}

Timer Timer::operator+(double seconds) const {
    return Timer(tp + std::chrono::steady_clock::duration(std::chrono::microseconds((int) (seconds * 1000000))));
}

void Timer::increment(double seconds) {
    tp += std::chrono::steady_clock::duration(std::chrono::microseconds((int) (seconds * 1000000)));
}

bool Timer::operator<(const Timer &rhs) const {
    return (tp < rhs.tp);
}

