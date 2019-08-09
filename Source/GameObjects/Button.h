//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_BUTTON_H
#define PONGCLONE_BUTTON_H

#include "../Components/GameObject.h"

class Button : public GameObject
{
public:
    Button() = default;
    ~Button() override = default;

    void setupButton(ASGE::Renderer *renderer, const std::string &file_name,
                     float x, float y, float width, float height);
};


#endif //PONGCLONE_BUTTON_H
