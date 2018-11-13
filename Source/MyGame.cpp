//
// Created by Huxy on 03/10/2018.
//

#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Platform.h>
#include <Engine/Sprite.h>
#include "MyGame.h"

MyGame::~MyGame()
{

}

bool MyGame::init()
{
    game_name = "Hello ASGE";

    if (!initAPI())
        return false;

    toggleFPS();
    return true;
}

void MyGame::update(const ASGE::GameTime &us)
{
    return;
}

void MyGame::render(const ASGE::GameTime &us)
{
    return;
}

