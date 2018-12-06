//
// Created by Zoe on 14/11/2018.
//

#ifndef HELLOASGE_PLAYER_H
#define HELLOASGE_PLAYER_H

#include <Engine/Sprite.h>


class Player
{
public:
    Player(float x_start, float y_start);

    // Setters
    void updateScore(int to_add);
    void moveUp();
    void moveDown();

    void yPos(float new_y);

    void reset();
    void resetSize();
    void resetSpeed();
    void resetPosition();
    void resetScore();

    // Getters
    float xPos();
    float yPos();

    float paddleSpeed();

    int playerScore();

    float paddleWidth();
    float paddleHeight();

    void initSprite(ASGE::Sprite* rawSprite);
    ASGE::Sprite* getSprite();
    void deleteSprite();

    void updatePosition();

private:
    float start_width = 20.0;
    float start_height = 120.0;
    float start_speed = 300;
    int score = 0;

    float start_x;
    float start_y;
    float x;
    float y;
    float width;
    float height;
    float speed;
    ASGE::Sprite* sprite = nullptr;
};


#endif //HELLOASGE_PLAYER_H
