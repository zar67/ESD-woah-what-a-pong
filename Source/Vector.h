//
// Created by Zoe on 14/11/2018.
//

#ifndef HELLOASGE_VECTOR_H
#define HELLOASGE_VECTOR_H

class Vector
{
public:
    Vector(float x_dir, float y_dir);

    // Setters
    void multiplyBy(float x_scale, float y_scale);
    void normalise();

    void xPos(float new_x);
    void yPos(float new_y);

    // Getters
    float xPos();
    float yPos();

private:
    float x;
    float y;
};

#endif //HELLOASGE_VECTOR_H