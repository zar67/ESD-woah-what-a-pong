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

    // Adds the to_add value on to the score.
    void increaseScore();

    // Initialises the sprite, loading in the texture and setting the size.
    void initSprite(ASGE::Sprite* rawSprite);
    // Deals with the deleting of the dynamic sprite, called in the game's destructor.
    void deleteSprite();
    void updatePosition();

    void reset();

    // Getters
    float xPos();
    float yPos();
    int playerScore();

    // Returns the sprite for rendering purposes.
    ASGE::Sprite* getSprite();

private:
    float start_x;
    float x;
    float start_y;
    float y;
    int score = 0;
    ASGE::Sprite* sprite = nullptr;
};

#endif //HELLOASGE_PLAYER_H