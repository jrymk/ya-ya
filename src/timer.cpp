#include "timer.h"
#include "debugger.h"
#include "graphics.h"
#include "utilities.h"
#include <sstream>
#include <iomanip>

Timer::Timer() { tp = std::chrono::steady_clock::now(); }
Timer::Timer(const Timer& timer) { tp = timer.tp; }
Timer::Timer(std::chrono::steady_clock::time_point tp): tp(tp) {}

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
    return Timer(tp + std::chrono::steady_clock::duration(std::chrono::microseconds((int)(seconds * 1000000))));
}

void Timer::increment(double seconds) {
    tp += std::chrono::steady_clock::duration(std::chrono::microseconds((int)(seconds * 1000000)));
}

bool Timer::operator<(const Timer& rhs) const {
    return (tp < rhs.tp);
}


void Profiler::timeSplit(const std::string& title) {
    splits.push_back(std::make_pair(Timer::getNow(), title));
}

void Profiler::drawBarAndClear() {
    timeSplit("end");
    // UIVec origin(50., 50.);
    
    for (int i = 0; i < splits.size() - 1; i++) {
        std::string title = splits[i].second;
        // Graphics::fillRect(sf::Color(double(title[0] - 'a') / 26. * 255., double(title[1] - 'a') / 26. * 255., double(title[2] - 'a') / 26. * 255.), origin + UIVec(10000. * splits[i].first.elapsed(splits[0].first), 0.), UIVec(10000. * splits[i+1].first.elapsed(splits[0].first), 20.));
        std::stringstream ss;
        ss << std::setw(20) << title << " " << std::setw(6) << int(splits[i].first.elapsed(splits[i+1].first) * 1000000.) << "us",

        Graphics::drawText(
            ss.str(),
            sf::Color(double(title[0] - 'a') / 26. * 255., double(title[1] - 'a') / 26. * 255., double(title[2] - 'a') / 26. * 255.),
            13., 
            UIVec(500., 20.) + UIVec(0., 16.) * i,
            0.,
            sf::Color(255, 255, 255, 150),
            2.
        );
        Graphics::fillRect(
            sf::Color(double(title[0] - 'a') / 26. * 255., double(title[1] - 'a') / 26. * 255., double(title[2] - 'a') / 26. * 255.),
            UIVec(750., 6.) + UIVec(0., 16.) * i,
            UIVec(750., 6.) + UIVec(0., 16.) * i + UIVec(splits[i].first.elapsed(splits[i+1].first) * 100000. ,16.)
        );
    }

    splits.resize(0);
}