//
// Created by Zoe on 14/11/2018.
//

#include "Player.h"

Player::Player(float x_start, float y_start)
{
    start_x = x_start;
    start_y = y_start;

    reset();
}

void Player::yPos(float new_y)
{
    y = new_y;
}

void Player::move(int m)
{
    moving = m;
}

void Player::increaseScore()
{
    score += 1;
}

void Player::initSprite(ASGE::Sprite* rawSprite)
{
    sprite = rawSprite;
    sprite->loadTexture("data/images/paddle.png");
    sprite->width(20);
    sprite->height(120);
    sprite->xPos(x);
    sprite->yPos(y);
}

void Player::deleteSprite()
{
    delete sprite;
    sprite = nullptr;
}

void Player::updatePosition(double delta)
{
    if (moving < 0)
    {
        y -= move_speed * delta;
    }
    else if (moving > 0)
    {
        y += move_speed * delta;
    }
    sprite->yPos(y);
}

void Player::reset()
{
    x = start_x;
    y = start_y;
    score = 0;
}

float Player::xPos()
{
    return x;
}

float Player::yPos()
{
    return y;
}

float Player::speed()
{
    return move_speed;
}

int Player::playerScore()
{
    return score;
}

ASGE::Sprite* Player::getSprite()
{
    return sprite;
}