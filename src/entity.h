#pragma once
#ifndef YAYA_ENTITY_H
#define YAYA_ENTITY_H

#include <memory>
#include "utilities.h"
#include "graphics.h"
#include "camera.h"
#include "timer.h"
#include "action.h"
#include "saveUtilities.h"
#include "map.h"
#include <deque>

#define OUT_OF_SIGHT 500
#define GRAVITY -2.

enum EntityType {
    ENTITY,
    PLAYER,
    DUCK,
    EGG,
    EGG_CARTON
};

class Game;

/// @brief Renderer and motion control for entities
class Entity {
protected:
    std::vector<std::shared_ptr<Entity>> inventory_last;

public:
    std::string id = "undefined";
    EntityType type = ENTITY;
    bool deleted = false;

    // game logic
    std::vector<std::shared_ptr<Entity>> inventory;
    std::shared_ptr<Entity> ownedBy = nullptr;
    int ownedSlot = 0;
    uint32_t seed;

    Timer entityTimer; // get elapsed per update
    double lastUpdate = -1.;
    double elapsedSecs = 0.;

    // interaction and appearance
    std::vector<Graphics::Quad> const* model;
    double zDepthOverride = -1e8; // threshold -100.
    float zDepthOffset = 0.;
    coord footprint = coord(1, 1);
    CollideBox collideBox = CollideBox({0., 0.}, {.7, .7}, true); // yes each entity only gets one collidable shape
    bool selectable = true;
    std::pair<int, int> neighborsFinderMyTile = {-1e8, 1e8};
    float opacity = 0.;
    float scale = 1.;

    // motion
    bool motionFrozen = false;
    bool collisionPushable = true;
    bool collisionCollidable = true;
    bool collisionNoEnv = false;
    coord position;
    std::deque<std::pair<Timer, coord>> historyPosition;
    double zPosition = 0.;
    double velocity = 0.;
    double zVelocity = 0.;
    coord slideVelocity; // TODO: get rid of this
    double heading = 0.;
    double headingRotationSpeed = 0.;


    void runActionEntity(Action &action, std::vector<Action> &followUpActions);

    virtual void runAction(Action &action, std::vector<Action> &followUpActions);

    virtual void loadModel();

    virtual void objInit();

    void pushQuads();

    void updateTimer();

    void update();

    virtual void environmentUpdate();

    virtual std::string getDescriptionStr();

    virtual void customUpdate();

    virtual void setInventoryProps();

    Entity();

    ~Entity();

    constexpr static auto properties = std::make_tuple(
            SaveUtilities::property(&Entity::id, "Ey.id"),
            SaveUtilities::property(&Entity::type, "Ey.tp"),
            SaveUtilities::property(&Entity::position, "Ey.ps"),
            SaveUtilities::property(&Entity::deleted, "Ey.dl"),
            SaveUtilities::property(&Entity::opacity, "Ey.op")
    );
};

#endif