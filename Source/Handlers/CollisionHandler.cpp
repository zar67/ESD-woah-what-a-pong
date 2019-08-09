//
// Created by Zoe on 08/08/2019.
//

#include <math.h>
#include <iostream>
#include "CollisionHandler.h"

const int SIDE_LEFT = 1;
const int SIDE_RIGHT = 2;
const int SIDE_TOP = 3;
const int SIDE_BOTTOM = 4;
const int INVALID = 0;

vector2 CollisionHandler::calculateNewDir(Ball* ball, Player* player)
{
    float prev_x = ball->direction().x;
    float half_player_height = player->height() / 2;

    // Calculate where the ball hit by a number from -1 to 1.
    float paddle_intersect = (player->position().y + half_player_height) - (ball->position().y + (ball->width() / 2));
    float normalised_intersect = paddle_intersect / half_player_height;

    // Convert the number to a radian value. 1.309 radians = 60 degrees
    double bounce_angle = normalised_intersect * radian_boundary;

    // Get the direction the ball should go in.
    auto new_x_dir = float(ball->movementComponent()->speed() * cos(bounce_angle));
    auto new_y_dir = float(ball->movementComponent()->speed() * sin(bounce_angle));

    vector2 new_dir = vector2(new_x_dir, new_y_dir);

    new_dir.normalise();
    return vector2(new_dir.x, new_dir.y);
}

vector2
CollisionHandler::AABBCircleCollision(Circle circle, Rectangle rectangle)
{
    float closest_x =
            fmaxf(rectangle.x, fminf((circle.x + circle.radius), (rectangle.x + rectangle.width)));
    float x_diff = (circle.x + circle.radius) - closest_x;

    float closest_y =
            fmaxf(rectangle.y, fminf((circle.y + circle.radius), (rectangle.y + rectangle.height)));
    float y_diff = (circle.y + circle.radius) - closest_y;

    if ((circle.radius * circle.radius) > ((x_diff * x_diff) + (y_diff * y_diff)))
    {
        return vector2(closest_x, closest_y);
    }
    return vector2(0, 0);
}

int CollisionHandler::getCollisionSide(vector2 point, Rectangle col_shape, vector2 ball_dir)
{
    if (point.x == col_shape.x && ball_dir.x > 0)
    {
        return SIDE_LEFT;
    }
    else if (point.x == col_shape.x + col_shape.width && ball_dir.x < 0)
    {
        return SIDE_RIGHT;
    }
    else if (point.y == col_shape.y && ball_dir.y > 0)
    {
        return SIDE_TOP;
    }
    else if (point.y == col_shape.y + col_shape.height && ball_dir.y < 0)
    {
        return SIDE_BOTTOM;
    }
    return INVALID;
}

vector2
CollisionHandler::collision(Ball* ball, Player* player_one, Player* player_two, Obstacle* obstacles, int obstacle_num, float screen_height)
{
    // Hit top boundary
    if (ball->position().y <= 0 && ball->direction().y < 0)
    {
        ball->setPosition(ball->position().x, 0);
        ball->setDirection(ball->direction().x, -ball->direction().y);
    }

    // Hit bottom boundary
    if (ball->position().y + ball->height() >= screen_height && ball->direction().y > 0)
    {
        ball->setPosition(ball->position().x, screen_height - ball->height());
        ball->setDirection(ball->direction().x, -ball->direction().y);
    }

    // Hit left paddle
    vector2 point = AABBCircleCollision(ball->getShape(), player_one->getShape());
    if (point.x != INVALID || point.y != INVALID)
    {
        int side = getCollisionSide(point, player_one->getShape(), ball->direction());

        switch (side)
        {
            case SIDE_RIGHT:
            {
                ball->setPosition(player_one->position().x + player_one->width(), ball->position().y);
                return calculateNewDir(ball, player_one);
            }
            case SIDE_TOP:
            {
                ball->setPosition(ball->position().x, player_one->position().y - ball->height());
                return vector2(ball->direction().x, -ball->direction().y);
            }
            case SIDE_BOTTOM:
            {
                ball->setPosition(ball->position().x, player_one->position().y + player_one->height());
                return vector2(ball->direction().x, -ball->direction().y);
            }
            default:
                return vector2(ball->direction().x, ball->direction().y);
        }
    }

    // Hit right paddle
    point = AABBCircleCollision(ball->getShape(), player_two->getShape());
    if (point.x != INVALID || point.y != INVALID)
    {
        int side = getCollisionSide(point, player_two->getShape(), ball->direction());

        switch (side)
        {
            case SIDE_LEFT:
            {
                ball->setPosition(player_two->position().x - ball->width(), ball->position().y);
                vector2 new_dir = calculateNewDir(ball, player_two);
                return vector2(-new_dir.x, new_dir.y);
            }
            case SIDE_TOP:
            {
                ball->setPosition(ball->position().x, player_two->position().y - ball->height());
                return vector2(ball->direction().x, -ball->direction().y);
            }
            case SIDE_BOTTOM:
            {
                ball->setPosition(ball->position().x, player_two->position().y + player_two->height());
                return vector2(ball->direction().x, -ball->direction().y);
            }

            default:
                return vector2(ball->direction().x, ball->direction().y);
        }
    }

    // Hit Any Obstacles
    for (int i = 0; i < obstacle_num; i++)
    {
        point = AABBCircleCollision(ball->getShape(), obstacles[i].getShape());
        if (point.x != INVALID || point.y != INVALID)
        {
            int side = getCollisionSide(point, obstacles[i].getShape(), ball->direction());

            switch (side)
            {
                case SIDE_LEFT:
                {
                    ball->setPosition(obstacles[i].position().x - ball->width(), ball->position().y);
                    return vector2(-ball->direction().x, ball->direction().y);
                }
                case SIDE_RIGHT:
                {
                    ball->setPosition(obstacles[i].position().x + obstacles[i].width(), ball->position().y);
                    return vector2(-ball->direction().x, ball->direction().y);
                }
                case SIDE_TOP:
                {
                    ball->setPosition(ball->position().x, obstacles[i].position().y - ball->height());
                    return vector2(ball->direction().x, -ball->direction().y);
                }
                case SIDE_BOTTOM:
                {
                    ball->setPosition(ball->position().x, obstacles[i].position().y + obstacles[i].height());
                    return vector2(ball->direction().x, -ball->direction().y);
                }
                default:
                    return vector2(ball->direction().x, ball->direction().y);
            }
        }
    }

    return ball->direction();
}

bool
CollisionHandler::isInside(const ASGE::Sprite *sprite, double x, double y) const
{
    return (x > sprite->xPos() &&
            x < (sprite->xPos()+sprite->width()) &&
            y > sprite->yPos() &&
            y < (sprite->yPos()+sprite->height()));
}
