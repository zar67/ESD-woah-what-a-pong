//
// Created by Zoe on 14/11/2018.
//

#include <iostream>
#include "Ball.h"

Ball::Ball()
{
    reset();
}

void Ball::ballSpeed(float new_speed)
{
    speed = new_speed;
}

void Ball::xPos(float new_x)
{
    x = new_x;
}

void Ball::yPos(float new_y)
{
    y = new_y;
}

void Ball::xDir(float new_x)
{
    direction.xPos(new_x);
}

void Ball::yDir(float new_y)
{
    direction.yPos(new_y);
}

void Ball::initSprite(ASGE::Sprite* rawSprite)
{
    sprite = rawSprite;
    sprite->loadTexture("data/images/ballBlue.png");
    sprite->width(size);
    sprite->height(size);
    sprite->xPos(x);
    sprite->yPos(y);
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

void Ball::reset()
{
    speed = 350;
    x = 503;
    y = 370;

    // Generates a random direction for the ball to go in.
    float new_x_dir = rand() % 3 - 3;
    float new_y_dir = rand() % 3 - 6;
    direction.xPos(new_x_dir);
    direction.yPos(new_y_dir);
    direction.normalise();
}

float Ball::ballSpeed()
{
    return speed;
}

float Ball::ballSize()
{
    return size;
}

float Ball::xPos()
{
    return x;
}

float Ball::yPos()
{
    return y;
}

float Ball::xDir()
{
    return direction.xPos();
}

float Ball::yDir()
{
    return direction.yPos();
}

ASGE::Sprite* Ball::getSprite()
{
    return sprite;
}