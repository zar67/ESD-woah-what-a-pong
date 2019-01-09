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
    void yPos(float new_y);
    void moveUp();
    void moveDown();

    // Adds the to_add value on to the score.
    void updateScore(int to_add);

    // Initialises the sprite, loading in the texture and setting the size.
    void initSprite(ASGE::Sprite* rawSprite);
    // Deals with the deleting of the dynamic sprite, called in the game's destructor.
    void deleteSprite();
    void updatePosition();

    void reset();

    // Getters
    float paddleSpeed();

    float paddleWidth();
    float paddleHeight();

    float xPos();
    float yPos();

    int playerScore();

    // Returns the sprite for rendering purposes.
    ASGE::Sprite* getSprite();

private:
    float start_speed = 300;
    float speed;

    float start_width = 20.0;
    float width;
    float start_height = 120.0;
    float height;

    float start_x;
    float x;
    float start_y;
    float y;

    int score = 0;

    ASGE::Sprite* sprite = nullptr;
};

#endif //HELLOASGE_PLAYER_H