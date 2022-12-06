#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include "utilities.h"
#include "graphics.h"
#include "camera.h"
#include "timer.h"
#include "action.h"
#include <deque>

#define OUT_OF_SIGHT 500
#define GRAVITY -1.

/// @brief Renderer and motion control for entities
class Entity {
public:
    std::string id = "undefined";
    std::string type = "entity";
    std::pair<int, int> neighborsFinderMyTile = {-1e8, 1e8};

    Timer entityTimer;
    double lastUpdate = -1.;
    double elapsedSecs = 0.;

    coord position;
    std::deque<std::pair<Timer, coord>> historyPosition;
    double zPosition = 0.;
    double velocity = 0.;
    double zVelocity = 0.;
    coord slideVelocity; // TODO: get rid of this
    double heading = 0.;
    double headingRotationSpeed = 0.;
    
    bool genderIsMale = true;
    bool fertilized = false;

    
    std::vector<Graphics::Quad> model;

    virtual void runAction(Action& action, std::vector<Action>& followUpActions);

    virtual void initModel();

    void pushQuads();

    void updateTimer();

    void update();

    virtual std::string getDescriptionStr();

    virtual void customUpdate();
};

#endif