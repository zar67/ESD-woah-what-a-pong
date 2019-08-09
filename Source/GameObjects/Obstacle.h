//
// Created by Zoe on 09/08/2019.
//

#ifndef PONGCLONE_OBSTACLE_H
#define PONGCLONE_OBSTACLE_H

#include <Engine/Sprite.h>
#include "../Components/GameObject.h"
#include "../Utility/Rectangle.h"

class Obstacle : public GameObject
{
public:
    Obstacle() = default;
    ~Obstacle() override = default;

    void setup(ASGE::Renderer *renderer, const std::string &file_name,
               float x_start, float y_start, float start_speed, float width,
               float height, float timer);

    // Setters
    void update(double delta_time, float screen_height);
    void generateNewDir();
    void reset();
    void moveTimer(float t);

    // Getters
    Rectangle getShape();
    float moveTimer();
    float waitTime();

private:
    float start_x = 0;
    float start_y = 0;

    float wait_time = 1000;
    float timer;

    vector2 dir = vector2(0,0);
};


#endif //PONGCLONE_OBSTACLE_H
