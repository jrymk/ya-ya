#ifndef _ACTION_H_
#define _ACTION_H_

#include <vector>
#include <stdarg.h>
#include "timer.h"

class Action {
public:
    bool ranFlag = false;
    bool deleteFlag = false;
    Timer time;
    std::string id;
    std::string action; // is this a good idea?
    bool rerun = false;

    inline Action() {}
    inline Action(const std::string& id, const Timer& time, const std::string& action)
    : id(id), time(time), action(action) {
    }

    inline bool operator<(const Action& rhs) {
        if (deleteFlag != rhs.deleteFlag)
            return rhs.deleteFlag;
        return !(time < rhs.time);
    }
};


#endif