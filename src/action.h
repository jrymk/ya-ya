#pragma once
#ifndef _ACTION_H_
#define _ACTION_H_

#include <vector>
#include <stdarg.h>
#include "timer.h"
#include "graphics.h"
#include "camera.h"

class Entity;

enum Command {
    INIT,
    GLOBAL_LAY_UNFERTILIZED_EGG,
    GLOBAL_LAY_FERTILIZED_EGG,
    GLOBAL_HATCH,
    GLOBAL_DESTROY,
    GLOBAL_PROCESS_COLLISION,
    GLOBAL_FIND_MATE_FEMALE,
    GLOBAL_LAY_EGG_FIND_NEARBY_MALE,
    DUCK_LOOP_WANDER,
    DUCK_LOOP_LAY_EGGS,
    DUCK_LOOP_FIND_MATE,
    DUCK_DEATH,
    DUCK_HOP,
    DUCK_HAVE_SEX_WITH,
    DUCK_DUCKWALK_TO_UNTIL,
    DUCK_SLIDE_VELOCITY_DISTANCE,
    DUCK_RESULT_FIND_MATE_FEMALE,
    DUCK_LAY_UNFERTILIZED_EGG,
    DUCK_LAY_FERTILIZED_EGG,
    DUCK_SLIDE_INSTANT,
    DUCK_SLIDE_VELOCITY,
    EGG_HATCH,
    EGG_HOP,
};

class Action {
public:
    bool ranFlag = false;
    bool deleteFlag = false;
    Timer time;

    Entity* entity;
    Command command;

    inline Action() {}
    inline Action(Entity* entity, const Timer& time, Command command)
    : entity(entity), time(time), command(command) {
    }

    bool argBool[8];
    int argInt[8];
    double argFloat[8];
    coord argCoord[8];
    UIVec argUIVec[8];
    Entity* argEntity[8];
    std::string argString[8];

    inline bool operator<(const Action rhs) const {
        if (deleteFlag != rhs.deleteFlag)
            return rhs.deleteFlag;
        return !(time < rhs.time);
    }
};


#endif