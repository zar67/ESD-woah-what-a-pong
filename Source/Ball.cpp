//
// Created by Zoe on 14/11/2018.
//

#include <iostream>
#include "Ball.h"

Ball::Ball()
{
    reset();
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

float Ball::xPos()
{
    return x;
}

float Ball::yPos()
{
    return y;
}

float Ball::ballSize()
{
    return size;
}

void Ball::multiplyVector(float x_scalar, float y_scalar)
{
    direction = direction.multiplyBy(x_scalar, y_scalar);
}

void Ball::xPos(float new_x)
{
    x = new_x;
}

void Ball::yPos(float new_y)
{
    y = new_y;
}

float Ball::xDir()
{
    return direction.xPos();
}

float Ball::yDir()
{
    return direction.yPos();
}

float Ball::ballSpeed()
{
    return speed;
}

void Ball::initSprite(ASGE::Sprite* rawSprite)
{
    sprite = rawSprite;
    sprite->loadTexture("data/images/ball.png");
    sprite->width(size);
    sprite->height(size);
    sprite->xPos(x);
    sprite->yPos(y);
}

ASGE::Sprite* Ball::getSprite()
{
    return sprite;
}

void Ball::deleteSprite()
{
    delete sprite;
    sprite = nullptr;
}

void Ball::updatePosition()
{
    sprite->xPos(x);
    sprite->yPos(y);
}

void Ball::ballSpeed(float new_speed)
{
    speed = new_speed;
}

void Ball::xDir(float new_x)
{
    direction.xPos(new_x);
}

void Ball::yDir(float new_y)
{
    direction.yPos(new_y);
}

void Ball::ballSize(float new_size)
{
    size = new_size;
}
