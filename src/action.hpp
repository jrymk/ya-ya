#ifndef _ACTION_HPP_
#define _ACTION_HPP_

#include <vector>
#include "timer.hpp"

class Action {
    std::vector<Action> actions;

    enum Type {} type;
};

#endif