//
// Created by Zoe on 08/08/2019.
//

#include "GameObject.h"

GameObject::~GameObject()
{
    freeSprite();
    freeMovement();
}

bool GameObject::addSpriteComponent(ASGE::Renderer* renderer, const std::string& texture_file_name)
{
    freeSprite();

    sprite_component = new SpriteComponent();
    if (sprite_component->loadSprite(renderer, texture_file_name))
    {
        return true;
    }

    freeSprite();
    return false;
}

bool GameObject::addMovementComponent(float initial_speed)
{
    freeMovement();

    movement_component = new MovementComponent(initial_speed);

    return false;
}

SpriteComponent* GameObject::spriteComponent()
{
    return sprite_component;
}

MovementComponent* GameObject::movementComponent()
{
    return movement_component;
}

bool GameObject::active()
{
    return visible;
}

void GameObject::active(bool a)
{
    visible = a;
}

void GameObject::freeSprite()
{
    delete sprite_component;
    sprite_component = nullptr;
}

void GameObject::freeMovement()
{
    delete movement_component;
    movement_component = nullptr;
}

vector2 GameObject::position()
{
    return vector2(sprite_component->getSprite()->xPos(),
                   sprite_component->getSprite()->yPos());
}

float GameObject::width()
{
    return sprite_component->getSprite()->width();
}

float GameObject::height()
{
    return sprite_component->getSprite()->height();
}
