//
// Created by Zoe on 14/11/2018.
//

#include <iostream>
#include "Ball.h"

Ball::Ball()
{
    reset();
}

void Ball::move()
{
    x += direction.xPos() * speed;
    y += direction.yPos() * speed;
}

void Ball::resetPosition()
{
    x = start_x;
    y = start_y;
}

void Ball::resetSize()
{
    size = start_size;
}

void Ball::resetSpeed()
{
    speed = start_speed;
}

void Ball::reset()
{
    resetPosition();
    resetSpeed();
    resetSize();

    float new_x_dir = rand() % 3 - 3;
    float new_y_dir = rand() % 3 - 3;
    direction.setValues(new_x_dir, new_y_dir);
    direction.normalise();
}

int Ball::xPos()
{
    return x;
}

int Ball::yPos()
{
    return y;
}

int Ball::ballSize()
{
    return size;
}

void Ball::multiplyVector(float x_scalar, float y_scalar)
{
    direction.multiplyBy(x_scalar, y_scalar);
}

void Ball::xPos(float new_x)
{
    x = new_x;
}

void Ball::yPos(float new_y)
{
    y = new_y;
}
