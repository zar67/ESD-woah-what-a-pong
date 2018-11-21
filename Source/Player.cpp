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

int Player::paddleWidth()
{
    return width;
}

int Player::paddleHeight()
{
    return height;
}

void Player::moveUp()
{
    y -= 20 * speed;
}

void Player::moveDown()
{
    y += 20 * speed;
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
    void resetSize();
    void resetSpeed();
    void resetPosition();
    void resetScore();
}

