//
// Created by Zoe on 08/08/2019.
//

#include "UIHandler.h"

void UIHandler::setupUIObjects(ASGE::Renderer *renderer, float width, float height)
{
    menu_play_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 412, 344, width, height);
    menu_quit_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 412, 394, width, height);

    options_1_player_button.setupButton(renderer, "data/Textures/UI/button_long_dark_grey_pressed.png", 302, 304, width, height);
    options_2_player_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 522, 304, width, height);
    options_easy_button.setupButton(renderer, "data/Textures/UI/button_long_dark_grey_pressed.png", 302, 354, width, height);
    options_hard_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 522, 354, width, height);
    options_start_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 412, 434, width, height);

    game_end_play_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 412, 501, width, height);
    game_end_menu_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 412, 551, width, height);
    game_end_quit_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 412, 601, width, height);

    pause_resume_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 412, 301, width, height);
    pause_menu_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 412, 351, width, height);
    pause_quit_button.setupButton(renderer, "data/Textures/UI/button_long_grey.png", 412, 401, width, height);
}
