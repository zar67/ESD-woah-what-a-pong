//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_PLAYER_H
#define PONGCLONE_PLAYER_H

#include <Engine/Sprite.h>
#include "../Components/GameObject.h"
#include "../Utility/Rectangle.h"

class Player : public GameObject
{
public:
    Player() = default;
    ~Player() override = default;

    void setup(ASGE::Renderer *renderer, const std::string &file_name,
               float x_start, float y_start, float start_speed, float width,
               float height);

    // Setters
    void move(int m);
    void increaseScore(int a);
    void update(double delta_time, float screen_height);
    void reset();

    // Getters
    int playerScore();
    Rectangle getShape();

private:
    float start_x = 0;
    float start_y = 0;

    int score = 0;
    int moving = 0;
};


#endif //PONGCLONE_PLAYER_H
