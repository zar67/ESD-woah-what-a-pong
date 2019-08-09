//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_GAMEOBJECT_H
#define PONGCLONE_GAMEOBJECT_H

#include <string>
#include "../Utility/Vector2.h"
#include "SpriteComponent.h"
#include "MovementComponent.h"

class GameObject
{
public:
    // Constructor and (virtual) Destructor
    GameObject() = default;
    virtual ~GameObject();

    // Component adding functions
    bool addSpriteComponent(ASGE::Renderer* renderer, const std::string& texture_file_name);
    bool addMovementComponent(float initial_speed);

    // The components
    SpriteComponent* spriteComponent();
    MovementComponent* movementComponent();

    // Other functions
    bool active();
    void active(bool a);

    vector2 position();
    float width();
    float height();

protected:
    void freeSprite();
    void freeMovement();

    SpriteComponent* sprite_component = nullptr;
    MovementComponent* movement_component = nullptr;

    bool visible = true;
};


#endif //PONGCLONE_GAMEOBJECT_H
