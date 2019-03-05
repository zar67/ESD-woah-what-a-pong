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
    void xPos(float new_x);
    void yPos(float new_y);
    void xDir(float new_x);
    void yDir(float new_y);

    // Initialises the sprite, loading in the texture and setting the size.
    void initSprite(ASGE::Sprite* rawSprite);
    // Deals with the deleting of the sprite, called in the game's destructor.
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
    float speed;
    float size = 26;
    float x = -1;
    float y = -1;
    Vector direction = Vector(0.0f,0.0f);
    ASGE::Sprite* sprite = nullptr;
};

#endif //HELLOASGE_BALL_H