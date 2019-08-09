//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_MOVEMENTCOMPONENT_H
#define PONGCLONE_MOVEMENTCOMPONENT_H

#include "../Utility/Vector2.h"

class MovementComponent
{
public:
    MovementComponent(float initial_speed);
    ~MovementComponent() = default;

    // Position setters
    vector2 updatePlayerPosition(double delta, float x, float y, int moving);
    vector2 updateBallPosition(double delta, float x, float y, vector2 dir);

    // Setters
    void reset();
    void increaseSpeed(float a);
    void setSpeed(float a);

    // Getters
    float speed();

private:
    float start_speed = 350;
    float move_speed;
};


#endif //PONGCLONE_MOVEMENTCOMPONENT_H
