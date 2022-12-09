#pragma once
#ifndef _ACTION_H_
#define _ACTION_H_

#include <vector>
#include <stdarg.h>
#include <memory>
#include "timer.h"
#include "graphics.h"
#include "camera.h"

class Entity;

enum Command {
    INIT,
    GLOBAL_LAY_UNFERTILIZED_EGG,
    GLOBAL_LAY_FERTILIZED_EGG,
    GLOBAL_HATCH,
    GLOBAL_DESTROY, // string id
    GLOBAL_PROCESS_COLLISION, // entity e, entity f
    GLOBAL_FIND_MATE_FEMALE,
    GLOBAL_LAY_EGG_FIND_NEARBY_MALE, // entity e
    ENTITY_MOTION_FROZEN, // bool frozen
    ENTITY_HIGHLIGHTABLE, // bool highlightable
    ENTITY_COLLISION_PUSHABLE, // bool pushable
    ENTITY_COLLISION_COLLIDABLE, // bool collidable
    ENTITY_MOVE_TO_INSTANT, // coord pos
    ENTITY_MOVE_TO_APPROACH, // coord pos, float speed
    ENTITY_HEADING_INSTANT, // float heading
    ENTITY_HOP,
    ENTITY_SLIDE_VELOCITY_DISTANCE, // coord vel, float distance
    ENTITY_SLIDE_INSTANT, // coord pos_delta
    ENTITY_SLIDE_VELOCITY, // coord vel
    DUCK_LOOP_WANDER,
    DUCK_LOOP_LAY_EGGS,
    DUCK_LOOP_FIND_MATE,
    DUCK_DEATH,
    DUCK_HAVE_SEX_WITH,
    DUCK_DUCKWALK_TO_UNTIL,
    DUCK_RESULT_FIND_MATE_FEMALE,
    DUCK_LAY_UNFERTILIZED_EGG,
    DUCK_LAY_FERTILIZED_EGG,
    EGG_HATCH,
};

class Action {
public:
    bool ranFlag = false;
    bool deleteFlag = false;
    Timer time;

    bool isGlobal = false;
    std::shared_ptr<Entity> entity;
    Command command;

    inline Action() {}
    inline Action(const Timer& time, Command command)
    : isGlobal(true), time(time), command(command) {
    }
    inline Action(std::shared_ptr<Entity>& entity, const Timer& time, Command command)
    : isGlobal(false), entity(entity), time(time), command(command) {
    }

    bool argBool[8];
    int argInt[8];
    double argFloat[8];
    coord argCoord[8];
    UIVec argUIVec[8];
    std::shared_ptr<Entity> argEntity[8];
    std::string argString[8];

    inline bool operator<(const Action rhs) const {
        if (deleteFlag != rhs.deleteFlag)
            return rhs.deleteFlag;
        return !(time < rhs.time);
    }
};


#endif