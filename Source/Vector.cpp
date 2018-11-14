//
// Created by Zoe on 14/11/2018.
//

#include <math.h>
#include "Vector.h"

Vector::Vector(float x_dir, float y_dir)
{
    x = x_dir;
    y = y_dir;
}

Vector Vector::multiplyBy(float x_scale, float y_scale)
{
    x *= x_scale;
    y *= y_scale;
    return Vector(x,y);
}

void Vector::setValues(float new_x, float new_y)
{
    x = new_x;
    y = new_y;
}

float Vector::xPos()
{
    return x;
}

float Vector::yPos()
{
    return y;
}

void Vector::normalise()
{
    float length = pow((pow(x,2)+pow(y,2)),0.5);
    x /= length;
    y /= length;
}