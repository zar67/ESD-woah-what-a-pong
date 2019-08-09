//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_VECTOR2_H
#define PONGCLONE_VECTOR2_H


struct vector2
{
    // Construction
    vector2(float dir_x, float dir_y);
    vector2(const vector2& rhs);

    // Value changing operations
    void setAs(float dir_x, float dir_y);
    void multiplyBy(float scalar);
    void multiplyBy(float scalar_x, float scalar_y);
    void increaseBy(float increase);
    void increaseBy(float inc_x, float inc_y);
    void decreaseBy(float decrease);
    void decreaseBy(float dec_x, float dec_y);

    // Math operations
    void normalise();
    float crossProduct(vector2& vector);
    float dotProduct(vector2& vector);
    float distance(float dest_x, float dest_y);
    float distance(vector2& destination);
    float length();

    // Data
    float x = 0;
    float y = 0;
};


#endif //PONGCLONE_VECTOR2_H
