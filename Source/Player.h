//
// Created by Zoe on 14/11/2018.
//

#ifndef HELLOASGE_PLAYER_H
#define HELLOASGE_PLAYER_H


class Player
{
public:
    Player(int x_start, int y_start);

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
    int xPos();
    int yPos();

    int playerScore();

    int paddleWidth();
    int paddleHeight();

private:
    float start_width = 20;
    float start_height = 120;
    float start_speed = 2;
    int score = 0;

    float start_x;
    float start_y;
    float x;
    float y;
    int width;
    int height;
    float speed;
};


#endif //HELLOASGE_PLAYER_H
