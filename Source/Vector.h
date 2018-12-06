//
// Created by Zoe on 14/11/2018.
//

#ifndef HELLOASGE_VECTOR_H
#define HELLOASGE_VECTOR_H


class Vector
{
public:
    Vector(float x_dir, float y_dir);
    Vector multiplyBy(float x_scale, float y_scale);
    void normalise();
    void setValues(float new_x, float new_y);

    float xPos();
    float yPos();

    void xPos(float new_x);
    void yPos(float new_y);

private:
    float x;
    float y;
};


#endif //HELLOASGE_VECTOR_H
