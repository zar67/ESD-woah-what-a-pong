//
// Created by Zoe on 14/11/2018.
//

#ifndef HELLOASGE_PLAYER_H
#define HELLOASGE_PLAYER_H


class Player
{
public:
    Player(float x_start, float y_start);

    // Setters
    void updateScore(int to_add);
    void moveUp();
    void moveDown();

    void reset();
    void resetSize();
    void resetSpeed();
    void resetPosition();
    void resetScore();

    // Getters
    float xPos();
    float yPos();

    int playerScore();

    float paddleWidth();
    float paddleHeight();

private:
    float start_width = 20.0;
    float start_height = 120.0;
    float start_speed = 2.0;
    int score = 0;

    float start_x;
    float start_y;
    float x;
    float y;
    float width;
    float height;
    float speed;
};


#endif //HELLOASGE_PLAYER_H
