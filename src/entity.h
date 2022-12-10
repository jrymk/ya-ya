#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <memory>
#include "utilities.h"
#include "graphics.h"
#include "camera.h"
#include "timer.h"
#include "action.h"
#include "serialization.h"
#include <deque>

#define OUT_OF_SIGHT 500
#define GRAVITY -1.

enum EntityType {
    ENTITY,
    PLAYER,
    DUCK,
    EGG,
};

/// @brief Renderer and motion control for entities
class Entity {
public:
    std::string id = "undefined";
    EntityType type = ENTITY;
    std::pair<int, int> neighborsFinderMyTile = {-1e8, 1e8};
    
    bool deleted = false;

    Timer entityTimer; // get elapsed per update
    double lastUpdate = -1.;
    double elapsedSecs = 0.;

    bool motionFrozen = false;
    bool collisionPushable = true;
    bool collisionCollidable = true;
    coord position;
    std::deque<std::pair<Timer, coord>> historyPosition;
    double zPosition = 0.;
    double velocity = 0.;
    double zVelocity = 0.;
    coord slideVelocity; // TODO: get rid of this
    double heading = 0.;
    double headingRotationSpeed = 0.;
    
    bool facingHighlightable = true;
    bool selectable = true;

    std::vector<Graphics::Quad> model;

    void runActionEntity(Action& action, std::vector<Action>& followUpActions);

    virtual void runAction(Action& action, std::vector<Action>& followUpActions);

    virtual void initModel();

    void pushQuads();

    void updateTimer();

    void update();

    virtual std::string getDescriptionStr();

    virtual void customUpdate();

    ~Entity();

    constexpr static auto properties = std::make_tuple(
        SaveUtilities::property(&Entity::position, "Ey.ps")
    );
};

#endif