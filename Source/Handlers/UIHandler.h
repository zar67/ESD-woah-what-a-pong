//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_UIHANDLER_H
#define PONGCLONE_UIHANDLER_H

#include <string>
#include "../GameObjects/Button.h"

struct UIHandler
{
    void setupUIObjects(ASGE::Renderer* renderer, float width, float height);

    Button menu_play_button;
    Button menu_quit_button;

    Button options_1_player_button;
    Button options_2_player_button;
    Button options_easy_button;
    Button options_hard_button;
    Button options_start_button;

    Button game_end_play_button;
    Button game_end_menu_button;
    Button game_end_quit_button;

    Button pause_resume_button;
    Button pause_menu_button;
    Button pause_quit_button;

    std::string pressed_sprite = "data/Textures/UI/button_long_dark_grey_pressed.png";
    std::string normal_sprite = "data/Textures/UI/button_long_grey.png";
};

#endif //PONGCLONE_UIHANDLER_H