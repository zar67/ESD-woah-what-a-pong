//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_COLLISIONCOMPONENT_H
#define PONGCLONE_COLLISIONCOMPONENT_H

#include <Engine/Sprite.h>
#include "../GameObjects/Ball.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Obstacle.h"
#include "../Utility/Vector2.h"
#include "../Utility/Circle.h"
#include "../Utility/Rectangle.h"

class CollisionHandler
{
public:
    CollisionHandler() = default;
    ~CollisionHandler() = default;

    vector2 calculateNewDir(Ball* ball, Player* player);

    vector2 AABBCircleCollision(Circle circle, Rectangle rectangle);
    int getCollisionSide(vector2 point, Rectangle col_shape, vector2 ball_dir);

    vector2 collision(Ball* ball, Player* player_one, Player* player_two, Obstacle* obstacles, int obstacle_num, float screen_height, bool hard);

    bool isInside(const ASGE::Sprite* sprite, double x, double y) const;

private:
    const float radian_boundary = 1.309;
};


#endif //PONGCLONE_COLLISIONCOMPONENT_H
