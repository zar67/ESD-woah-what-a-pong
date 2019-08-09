//
// Created by Zoe on 08/08/2019.
//

#include "Ball.h"

void Ball::setup(ASGE::Renderer *renderer, const std::string &file_name,
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

    generateDirection();

    addMovementComponent(start_speed);
}

void Ball::update(double delta_time)
{
    movement_component->increaseSpeed(0.01f);

    // Move Ball
    vector2 new_pos = movement_component->updateBallPosition(delta_time, sprite_component->getSprite()->xPos(), sprite_component->getSprite()->yPos(), dir);
    sprite_component->getSprite()->xPos(new_pos.x);
    sprite_component->getSprite()->yPos(new_pos.y);
}

void Ball::reset()
{
    movement_component->reset();

    sprite_component->getSprite()->xPos(start_x);
    sprite_component->getSprite()->yPos(start_y);

    generateDirection();
}

void Ball::setDirection(float x, float y)
{
    dir.setAs(x, y);
    dir.normalise();
}

void Ball::setPosition(float x, float y)
{
    sprite_component->getSprite()->xPos(x);
    sprite_component->getSprite()->yPos(y);
}

void Ball::generateDirection()
{
    float new_x_dir = rand() % 3 - 3;
    float new_y_dir = rand() % 2 - 1;

    setDirection(new_x_dir, new_y_dir);
    dir.normalise();
}

vector2 Ball::direction()
{
    return dir;
}

Circle Ball::getShape()
{
    Circle shape;
    shape.setup(position().x, position().y, width() / 2);
    return shape;
}
