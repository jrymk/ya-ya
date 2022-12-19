#include "utilities.h"
#include "graphics.h"
#include "names.h"
#include <iomanip>

std::string toStr(int a) {
    std::stringstream ss;
    ss << a;
    return ss.str();
}

std::wstring toWStr(int a) {
    std::wstringstream ss;
    ss << a;
    return ss.str();
}

std::string toStr(double a, const int digits) {
    std::stringstream ss;
    if (digits >= 0)
        ss << std::fixed << std::setw(8) << std::setprecision(digits);
    ss << a;
    return ss.str();
}

std::pair<std::string, std::string> splitId(std::string id) {
    auto rbrckt = id.find('$');
    if (rbrckt == id.npos || id.length() < 2)
        return std::make_pair("", id);
    return std::make_pair(id.substr(0, rbrckt), id.substr(rbrckt + 1));
}

int FramerateCounter::getFramerateAndUpdate() {
    frametime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - prevFrame).count();
    prevFrame = std::chrono::steady_clock::now();
    frametimes.push(std::chrono::steady_clock::now());
    while (!frametimes.empty() && std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - frametimes.front()).count() >= 1000000)
        frametimes.pop();
    return frametimes.size();
}

float FramerateCounter::getFrametimeMs() {
    return (float) frametime / 1000;
}

FramerateCounter::FramerateCounter() {
    prevFrame = std::chrono::steady_clock::now();
}

std::mt19937 rng(time(0));

double getRand() {
    return std::max(0., std::min(1., (double) rng() / rng.max()));
}

uint32_t getRandInt() {
    return rng();
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