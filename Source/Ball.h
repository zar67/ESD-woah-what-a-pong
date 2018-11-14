//
// Created by Zoe on 14/11/2018.
//

#ifndef HELLOASGE_BALL_H
#define HELLOASGE_BALL_H

#include "Vector.h"


class Ball
{
public:
    Ball();

    // Setters
    void move();
    void multiplyVector(float x_scalar, float y_scalar);

    void reset();
    void resetPosition();
    void resetSize();
    void resetSpeed();

    // Getters
    int xPos();
    int yPos();
    int ballSize();

private:
    float start_x = 503;
    float start_y = 370;
    int start_size = 26;
    float start_speed = 0.5;

    float x;
    float y;
    float speed;
    float size;
    Vector direction = Vector(0,0);
};


#endif //HELLOASGE_BALL_H
