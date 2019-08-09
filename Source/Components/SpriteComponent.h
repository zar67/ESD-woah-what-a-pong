//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_SPRITECOMPONENT_H
#define PONGCLONE_SPRITECOMPONENT_H

#include <Engine/Sprite.h>

class SpriteComponent
{
public:
    SpriteComponent() = default;
    ~SpriteComponent();

    bool loadSprite(ASGE::Renderer* renderer, const std::string& file_name);
    ASGE::Sprite* getSprite();

private:
    void free();
    ASGE::Sprite* sprite = nullptr;
};


#endif //PONGCLONE_SPRITECOMPONENT_H
