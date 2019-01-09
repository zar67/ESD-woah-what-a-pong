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

void Player::moveUp()
{
    y -= speed / 10;
}

void Player::moveDown()
{
    y += speed / 10;
}

void Player::updateScore(int to_add)
{
    score += to_add;
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

void Player::reset()
{
    speed = start_speed;
    width = start_width;
    height = start_height;
    x = start_x;
    y = start_y;
    score = 0;
}

float Player::paddleSpeed()
{
    return speed;
}

float Player::paddleWidth()
{
    return width;
}

float Player::paddleHeight()
{
    return height;
}

float Player::xPos()
{
    return x;
}

float Player::yPos()
{
    return y;
}

int Player::playerScore()
{
    return score;
}

ASGE::Sprite* Player::getSprite()
{
    return sprite;
}