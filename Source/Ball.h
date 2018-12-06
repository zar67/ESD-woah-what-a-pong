//
// Created by Zoe on 14/11/2018.
//

#ifndef HELLOASGE_BALL_H
#define HELLOASGE_BALL_H

#include <Engine/Sprite.h>
#include "Vector.h"


class Ball
{
public:
    Ball();

    // Setters
    void multiplyVector(float x_scalar, float y_scalar);

    void reset();
    void resetPosition();
    void resetSize();
    void resetSpeed();

    void ballSpeed(float newSpeed);

    void xPos(float new_x);
    void yPos(float new_y);

    // Getters
    float xPos();
    float yPos();
    float ballSize();
    float xDir();
    float yDir();
    float ballSpeed();

    void initSprite(ASGE::Sprite* rawSprite);
    ASGE::Sprite* getSprite();
    void deleteSprite();

    void updatePosition();

private:
    float start_x = 503.0;
    float start_y = 370.0;
    float start_size = 26.0;
    float start_speed = 300.0;

    float x;
    float y;
    float speed;
    float size;
    Vector direction = Vector(0.0,0.0);
    ASGE::Sprite* sprite = nullptr;
};


#endif //HELLOASGE_BALL_H
