//
// Created by Zoe on 08/08/2019.
//

#include "Player.h"

void Player::setup(ASGE::Renderer *renderer, const std::string &file_name,
                   float x_start, float y_start, float start_speed, float width,
                   float height)
{
    start_x = x_start;
    start_y = y_start;

    addSpriteComponent(renderer, file_name);
    sprite_component->getSprite()->xPos(x_start);
    sprite_component->getSprite()->yPos(y_start);
    sprite_component->getSprite()->width(width);
    sprite_component->getSprite()->height(height);

    addMovementComponent(start_speed);
}

void Player::move(int m)
{
    moving = m;
}

void Player::increaseScore(int a)
{
    score += a;
}

void Player::update(double delta_time, float screen_height)
{
    if (sprite_component->getSprite()->yPos() + sprite_component->getSprite()->height() > screen_height)
    {
        moving = 0;
        sprite_component->getSprite()->yPos(screen_height - 1 - sprite_component->getSprite()->height());
    }
    else if (sprite_component->getSprite()->yPos() < 0)
    {
        moving = 0;
        sprite_component->getSprite()->yPos(1);
    }

    vector2 new_pos = movement_component->updatePlayerPosition(delta_time, sprite_component->getSprite()->xPos(), sprite_component->getSprite()->yPos(), moving);
    sprite_component->getSprite()->yPos(new_pos.y);
}

void Player::reset()
{
    movement_component->reset();

    sprite_component->getSprite()->xPos(start_x);
    sprite_component->getSprite()->yPos(start_y);

    score = 0;
}

int Player::playerScore()
{
    return score;
}

Rectangle Player::getShape()
{
    Rectangle shape;
    shape.setup(position().x, position().y, width(), height());
    return shape;
}


