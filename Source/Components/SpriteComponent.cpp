//
// Created by Zoe on 08/08/2019.
//

#include "SpriteComponent.h"
#include <Engine/Renderer.h>

SpriteComponent::~SpriteComponent()
{
    free();
}

bool SpriteComponent::loadSprite(ASGE::Renderer *renderer,
                                 const std::string &file_name)
{
    free();
    sprite = renderer->createRawSprite();
    if (sprite->loadTexture(file_name))
    {
        return true;
    }

    free();
    return false;
}

ASGE::Sprite *SpriteComponent::getSprite()
{
    return sprite;
}

void SpriteComponent::free()
{
    delete sprite;
    sprite = nullptr;
}
