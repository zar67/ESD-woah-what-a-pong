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
    void ballSpeed(float new_speed);
    void ballSize(float new_size);

    void xPos(float new_x);
    void yPos(float new_y);

    void xDir(float new_x);
    void yDir(float new_y);

    // Multiplies the direction vector by the scalars passed into the function.
    void multiplyVector(float x_scalar, float y_scalar);

    // Initialises the sprite, loading in the texture and setting the size.
    void initSprite(ASGE::Sprite* rawSprite);
    // Deals with the deleting of the dynamic sprite, called in the game's destructor.
    void deleteSprite();
    void updatePosition();

    void reset();

    // Getters
    float ballSpeed();
    float ballSize();

    float xPos();
    float yPos();

    float xDir();
    float yDir();

    // Returns the sprite for rendering purposes.
    ASGE::Sprite* getSprite();

private:
    float start_speed = 350.0;
    float speed;

    float start_size = 26.0;
    float size;

    float start_x = 503.0;
    float x;

    float start_y = 370.0;
    float y;

    Vector direction = Vector(0.0f,0.0f);

    ASGE::Sprite* sprite = nullptr;
};

#endif //HELLOASGE_BALL_H