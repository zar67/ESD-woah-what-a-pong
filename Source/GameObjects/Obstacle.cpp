//
// Created by Zoe on 09/08/2019.
//

#include <iostream>
#include "Obstacle.h"

void Obstacle::setup(ASGE::Renderer *renderer, const std::string &file_name,
                     float x_start, float y_start, float start_speed,
                     float width, float height, float timer)
{
    start_x = x_start;
    start_y = y_start;

    addSpriteComponent(renderer, file_name);
    sprite_component->getSprite()->xPos(x_start);
    sprite_component->getSprite()->yPos(y_start);
    sprite_component->getSprite()->width(width);
    sprite_component->getSprite()->height(height);

    addMovementComponent(start_speed);
    wait_time = timer;
}

void Obstacle::update(double delta_time, float screen_height)
{
    if (sprite_component->getSprite()->yPos() + sprite_component->getSprite()->height() > screen_height)
    {
        dir.y = 0;
        sprite_component->getSprite()->yPos(screen_height - 1 - sprite_component->getSprite()->height());
    }
    else if (sprite_component->getSprite()->yPos() < 0)
    {
        dir.y = 0;
        sprite_component->getSprite()->yPos(1);
    }

    vector2 new_pos = movement_component->updateBallPosition(delta_time, sprite_component->getSprite()->xPos(), sprite_component->getSprite()->yPos(), dir);
    sprite_component->getSprite()->yPos(new_pos.y);
}

void Obstacle::generateNewDir()
{
    int direction = rand() % 3 - 1;
    dir.y = (rand() % 6) * direction;
    dir.normalise();

    movementComponent()->setSpeed(rand() % 100 + 150);
}

void Obstacle::reset()
{
    movement_component->reset();

    sprite_component->getSprite()->xPos(start_x);
    sprite_component->getSprite()->yPos(start_y);
}

void Obstacle::moveTimer(float t)
{
    timer = t;
}

Rectangle Obstacle::getShape()
{
    Rectangle shape;
    shape.setup(position().x, position().y, width(), height());
    return shape;
}

float Obstacle::moveTimer()
{
    return timer;
}

float Obstacle::waitTime()
{
    return wait_time;
}
