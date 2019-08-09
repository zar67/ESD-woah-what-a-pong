//
// Created by Zoe on 08/08/2019.
//

#include "MovementComponent.h"

MovementComponent::MovementComponent(float initial_speed)
{
    start_speed = initial_speed;

    reset();
}

vector2 MovementComponent::updatePlayerPosition(double delta, float x, float y, int moving)
{
    if (moving < 0)
    {
        y -= move_speed * delta;
    }
    else if (moving > 0)
    {
        y += move_speed * delta;
    }

    return vector2(x,y);
}

vector2 MovementComponent::updateBallPosition(double delta, float x, float y, vector2 dir)
{
    float new_x = x + float(move_speed * dir.x * delta);
    float new_y = y + float(move_speed * dir.y * delta);

    return vector2(new_x,new_y);
}

void MovementComponent::reset()
{
    move_speed = start_speed;
}

void MovementComponent::increaseSpeed(float a)
{
    move_speed += a;
}

void MovementComponent::setSpeed(float a)
{
    move_speed = a;
}

float MovementComponent::speed()
{
    return move_speed;
}
