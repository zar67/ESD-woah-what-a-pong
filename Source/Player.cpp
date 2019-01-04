//
// Created by Zoe on 14/11/2018.
//

#include "Player.h"

Player::Player(float x_start, float y_start)
{
    start_x = x_start;
    start_y = y_start;

    resetPosition();
    resetSize();
    resetSpeed();
    resetScore();
}

void Player::updateScore(int to_add)
{
    score += to_add;
}

void Player::resetSize()
{
    width = start_width;
    height = start_height;
}

void Player::resetSpeed()
{
    speed = start_speed;
}

void Player::resetPosition()
{
    x = start_x;
    y = start_y;
}

float Player::xPos()
{
    return x;
}

float Player::yPos()
{
    return y;
}

float Player::paddleWidth()
{
    return width;
}

float Player::paddleHeight()
{
    return height;
}

void Player::moveUp()
{
    y -= speed / 10;
}

void Player::moveDown()
{
    y += speed / 10;
}

int Player::playerScore()
{
    return score;
}

void Player::resetScore()
{
    score = 0;
}

void Player::reset()
{
    resetSize();
    resetSpeed();
    resetPosition();
    resetScore();
}

void Player::initSprite(ASGE::Sprite* rawSprite)
{
    sprite = rawSprite;
    sprite->loadTexture("data/images/paddle.png");
    sprite->width(width);
    sprite->height(height);
    sprite->xPos(x);
    sprite->yPos(y);
}

ASGE::Sprite* Player::getSprite()
{
    return sprite;
}

void Player::deleteSprite()
{
    delete sprite;
    sprite = nullptr;
}

void Player::updatePosition()
{
    sprite->xPos(x);
    sprite->yPos(y);
}

float Player::paddleSpeed()
{
    return speed;
}

void Player::yPos(float new_y)
{
    y = new_y;
}

void Player::lastPos(float d)
{
    lastY = d;
}

float Player::lastPos()
{
    return lastY;
}
