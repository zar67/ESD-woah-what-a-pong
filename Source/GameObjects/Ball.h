//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_BALL_H
#define PONGCLONE_BALL_H

#include <Engine/Sprite.h>
#include "../Components/GameObject.h"
#include "../Utility/Vector2.h"
#include "../Utility/Circle.h"

class Ball : public GameObject
{
public:
    Ball() = default;
    ~Ball() override = default;

    void setup(ASGE::Renderer *renderer, const std::string &file_name,
               float x_start, float y_start, float start_speed, float width,
               float height);

    // Setters
    void update(double delta_time);
    void reset();
    void setDirection(float x, float y);
    void setPosition(float x, float y);
    void generateDirection();

    // Getters
    vector2 direction();
    Circle getShape();

private:
    float start_x = 0;
    float start_y = 0;

    vector2 dir = vector2(0,0);
};


#endif //PONGCLONE_BALL_H
