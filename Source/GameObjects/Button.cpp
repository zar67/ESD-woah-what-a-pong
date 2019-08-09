//
// Created by Zoe on 08/08/2019.
//

#include <iostream>
#include "Button.h"

void Button::setupButton(ASGE::Renderer *renderer, const std::string &file_name,
               float x, float y, float width, float height)
{
    addSpriteComponent(renderer, file_name);
    sprite_component->getSprite()->xPos(x);
    sprite_component->getSprite()->yPos(y);
    sprite_component->getSprite()->width(width);
    sprite_component->getSprite()->height(height);
}