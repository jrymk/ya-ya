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
    GLOBAL_DESTROY, // entity e
    ENTITY_OWN_BY, // entity e, int slot
    ENTITY_UNOWN,
    ENTITY_INVENTORY_ON_CAPTURE, // int slot, entity e
    ENTITY_INVENTORY_ON_RELEASE, // int slot, entity e
    ENTITY_MOTION_FROZEN, // bool frozen
    ENTITY_SELECTABLE, // bool selectable
    ENTITY_COLLISION_PUSHABLE, // bool pushable
    ENTITY_COLLISION_COLLIDABLE, // bool collidable
    ENTITY_MOVE_TO_INSTANT, // coord pos
    ENTITY_MOVE_TO_APPROACH, // coord pos, float speed
    ENTITY_ZPOS_TO_APPROACH_UNTIL, // float zpos, float speed
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
    DUCK_DUCKWALK_TO_UNTIL, // coord pos, float timeout
    DUCK_DUCKWALK_TO_DUCK, // entity e, float timeout
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
            : isGlobal(true), time(time), command(command),
            argBool(8), argInt(8), argFloat(8), argCoord(8), argUIVec(8), argEntity(8), argString(8) {
    }

    inline Action(const Timer &time, Command command, const std::string &caller)
            : isGlobal(true), time(time), command(command), caller(caller),
            argBool(8), argInt(8), argFloat(8), argCoord(8), argUIVec(8), argEntity(8), argString(8) {
    }

    inline Action(const std::shared_ptr<Entity> &entity, const Timer &time, Command command)
            : isGlobal(false), entity(entity), time(time), command(command),
            argBool(8), argInt(8), argFloat(8), argCoord(8), argUIVec(8), argEntity(8), argString(8) {
    }

    inline Action(const std::shared_ptr<Entity> &entity, const Timer &time, Command command, const std::string &caller)
            : isGlobal(false), entity(entity), time(time), command(command), caller(caller),
            argBool(8), argInt(8), argFloat(8), argCoord(8), argUIVec(8), argEntity(8), argString(8) {
    }

    std::vector<char> argBool;  // due to std::vector<bool> problem
    std::vector<int> argInt;
    std::vector<double> argFloat;
    std::vector<coord> argCoord;
    std::vector<UIVec> argUIVec;
    std::vector<std::shared_ptr<Entity> > argEntity;
    std::vector<std::string> argString;

    inline bool operator<(const Action &rhs) const {
        if (deleteFlag != rhs.deleteFlag)
            return rhs.deleteFlag;
        return !(time < rhs.time);
    }

    constexpr static auto properties = std::make_tuple(
        SaveUtilities::property(&Action::ranFlag, "Ac.rf"),
        SaveUtilities::property(&Action::deleteFlag, "Ac.df"),
        SaveUtilities::property(&Action::time, "Ac.ti"),
        SaveUtilities::property(&Action::caller, "Ac.cl"),
        SaveUtilities::property(&Action::isGlobal, "Ac.ig"),
        SaveUtilities::property(&Action::entity, "Ac.ey"),
        SaveUtilities::property(&Action::command, "Ac.cmd"),
        SaveUtilities::property(&Action::argBool, "Ac.ab"),
        SaveUtilities::property(&Action::argInt, "Ac.ai"),
        SaveUtilities::property(&Action::argFloat, "Ac.af"),
        SaveUtilities::property(&Action::argCoord, "Ac.acd"),
        SaveUtilities::property(&Action::argUIVec, "Ac.auv"),
        SaveUtilities::property(&Action::argEntity, "Ac.aey"),
        SaveUtilities::property(&Action::argString, "Ac.as")
    );
};


#endif