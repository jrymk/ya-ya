#ifndef _ACTION_HPP_
#define _ACTION_HPP_

#include <vector>
#include <stdarg.h>
#include "timer.hpp"

class Action {
public:
    bool deleteFlag = false;
    Timer time;
    std::string id;
    std::string action; // is this a good idea?
    bool rerun = false;

    Action() {}
    Action(const std::string& id, const Timer& time, const std::string& action)
    : id(id), time(time), action(action) {
    }

    bool operator<(const Action& rhs) {
        if (deleteFlag != rhs.deleteFlag)
            return rhs.deleteFlag;
        return !(time < rhs.time);
    }
};


#endif