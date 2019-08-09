//
// Created by Zoe on 08/08/2019.
//

#include "AIHandler.h"

void AIHandler::setup(ASGE::Renderer *renderer, const std::string &file_name,
                     float x_start, float y_start, float start_speed,
                     float width, float height)
{
    ai_ball.setup(renderer, file_name, x_start, y_start, start_speed, width,
                  height);
}

void AIHandler::update(double delta_time)
{
    ai_ball.update(delta_time);
}

void AIHandler::reset(vector2 pos, vector2 dir, float speed)
{
    ai_ball.spriteComponent()->getSprite()->xPos(pos.x);
    ai_ball.spriteComponent()->getSprite()->yPos(pos.y);
    ai_ball.setDirection(dir.x, dir.y);
    ai_ball.movementComponent()->setSpeed(speed);
}

bool AIHandler::track()
{
    return track_ball;
}

void AIHandler::track(bool t)
{
    track_ball = t;
}

float AIHandler::moveSecondPaddle(double delta, vector2 player_position, float speed, float height, float screen_height)
{
    float middlePos = player_position.y + (height / 2);
    if (ai_ball.position().y < middlePos && player_position.y > 0)
    {
        return float(player_position.y - (speed * delta));
    }
    else if (ai_ball.position().y > middlePos && player_position.y + height < screen_height)
    {
        return float(player_position.y + (speed * delta));
    }

    return player_position.y;
}

Ball* AIHandler::getAIBall()
{
    return &ai_ball;
}
