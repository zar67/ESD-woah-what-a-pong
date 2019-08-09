//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_AIHANDLER_H
#define PONGCLONE_AIHANDLER_H

#include "../GameObjects/Ball.h"

class AIHandler
{
public:
    AIHandler() = default;
    ~AIHandler() = default;

    void setup(ASGE::Renderer *renderer, const std::string &file_name,
               float x_start, float y_start, float start_speed, float width,
               float height);

    void update(double delta_time);
    void reset(vector2 pos, vector2 dir, float speed);

    float moveSecondPaddle(double delta, vector2 player_position, float speed, float height, float screen_height);

    bool track();
    void track(bool t);

    Ball* getAIBall();

private:
    Ball ai_ball;

    bool track_ball= false;
};


#endif //PONGCLONE_AIHANDLER_H
