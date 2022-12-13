#pragma once
#ifndef YAYA_ACTION_H
#define YAYA_ACTION_H

#include <vector>
#include <memory>
#include "timer.h"
#include "graphics.h"
#include "camera.h"

class Entity;

enum Command {
    ON_CREATION,
    ON_UNOWNED, // entity unowned_by, int slot
    GLOBAL_PROCESS_COLLISION, // entity e, entity f
    GLOBAL_DESTROY, // entity e
    ENTITY_OWN_BY, // entity e, int slot
    ENTITY_UNOWN,
    ENTITY_MOTION_FROZEN, // bool frozen
    ENTITY_SELECTABLE, // bool selectable
    ENTITY_COLLISION_PUSHABLE, // bool pushable
    ENTITY_COLLISION_COLLIDABLE, // bool collidable
    ENTITY_MOVE_TO_INSTANT, // coord pos
    ENTITY_MOVE_TO_APPROACH, // coord pos, float speed
    ENTITY_HEADING_INSTANT, // float heading
    ENTITY_HOP,
    ENTITY_SLIDE_VELOCITY_DISTANCE, // coord vel, float distance
    ENTITY_SLIDE_INSTANT, // coord pos_delta
    ENTITY_SLIDE_VELOCITY, // coord vel
    ENTITY_WALK_INSTANT, // float distance
    DUCK_LOOP_WANDER,
    DUCK_LOOP_LAY_EGGS,
    DUCK_LOOP_FIND_MATE,
    DUCK_DEATH,
    DUCK_DUCKWALK_TO_UNTIL,
    DUCK_DUCKWALK_TO_DUCK,
    DUCK_UNTIL_MATE_CONTACT, // entity e, entity mate
    DUCK_HAVE_SEX_WITH, // entity with
    DUCK_FINISH_SEX, // entity with
    EGG_FORM_EMBRYO,
};

class Action {
public:
    bool ranFlag = false;
    bool deleteFlag = false;
    Timer time;
    std::string caller = "";

    bool isGlobal = false;
    std::shared_ptr<Entity> entity;
    Command command;

    inline Action() = default;

    inline Action(const Timer &time, Command command)
            : isGlobal(true), time(time), command(command) {
    }

    inline Action(const Timer &time, Command command, const std::string &caller)
            : isGlobal(true), time(time), command(command), caller(caller) {
    }

    inline Action(std::shared_ptr<Entity> &entity, const Timer &time, Command command)
            : isGlobal(false), entity(entity), time(time), command(command) {
    }

    inline Action(std::shared_ptr<Entity> &entity, const Timer &time, Command command, const std::string &caller)
            : isGlobal(false), entity(entity), time(time), command(command), caller(caller) {
    }

    bool argBool[8] = {0};
    int argInt[8] = {0};
    double argFloat[8] = {0};
    coord argCoord[8];
    UIVec argUIVec[8];
    std::shared_ptr<Entity> argEntity[8];
    std::string argString[8];

    inline bool operator<(const Action &rhs) const {
        if (deleteFlag != rhs.deleteFlag)
            return rhs.deleteFlag;
        return !(time < rhs.time);
    }
};


#endif