//
// Created by Zoe on 08/08/2019.
//

#include <string>
#include <iostream>
#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Platform.h>
#include <Engine/Sprite.h>
#include "Game.h"

const int MENU_SCREEN = 0;
const int OPTIONS_SCREEN = 1;
const int GAME_SCREEN = 2;
const int GAME_OVER_SCREEN = 3;
const int PAUSE_SCREEN = 4;

const int BUTTON_HEIGHT = 30;
const int BUTTON_WIDTH = 200;

Game::~Game()
{
    this->inputs->unregisterCallback(key_callback_id);
    this->inputs->unregisterCallback(mouse_callback_id);
}

bool Game::init()
{
    game_width = 1024;
    game_height = 768;
    game_name = "PING";

    // Return false is API not initialised correctly
    if (!initAPI())
    {
        return false;
    }

    renderer->setSpriteMode(ASGE::SpriteSortMode::IMMEDIATE);
    renderer->setClearColour(ASGE::COLOURS::BLACK);
    inputs->use_threads = false;

    key_callback_id = inputs->addCallbackFnc(ASGE::E_KEY, &Game::keyHandler, this);
    mouse_callback_id =inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &Game::clickHandler, this);

    // Init GameObjects
    ball.setup(renderer.get(), "data/Textures/ball_grey.png", 503, 370, 350, 20,
               20);
    player_one.setup(renderer.get(), "data/Textures/paddle.png", 50, 324, 350,
                     25, 120);
    player_two.setup(renderer.get(), "data/Textures/paddle.png", 954, 324, 350,
                     25, 120);

    ai_handler.setup(renderer.get(), "data/Textures/ball_yellow.png", 503, 370, 350, 16, 16);
    ui_elements.setupUIObjects(renderer.get(), BUTTON_WIDTH, BUTTON_HEIGHT);

    // Init Obstacles
    obstacle_timer += OBSTACLE_WAIT_TIME;
    obstacles[0].setup(renderer.get(), "data/Textures/Blocks/tile_blue_rectangle.png", 287, game_height - 110, 200, 50, 100, 1000);
    obstacles[1].setup(renderer.get(), "data/Textures/Blocks/tile_blue_square.png", 387, 10, 200, 50, 50, 800);
    obstacles[2].setup(renderer.get(), "data/Textures/Blocks/tile_blue_square.png", 587, 10, 200, 50, 50, 800);
    obstacles[3].setup(renderer.get(), "data/Textures/Blocks/tile_blue_rectangle.png", 687, game_height - 110, 200, 50, 100, 1000);

    toggleFPS();
    return true;
}

void Game::keyHandler(const ASGE::SharedEventData data)
{
    auto key = static_cast<const ASGE::KeyEvent*>(data.get());

    if (screen_open == GAME_SCREEN)
    {
        if (key->key == ASGE::KEYS::KEY_ESCAPE && key->action == ASGE::KEYS::KEY_PRESSED)
        {
            screen_open = PAUSE_SCREEN;
        }
        else if (key->key == ASGE::KEYS::KEY_W)
        {
            if (key->action == ASGE::KEYS::KEY_RELEASED)
            {
                player_one.move(0);
            }
            else
            {
                player_one.move(-1);
            }
        }
        else if (key->key == ASGE::KEYS::KEY_S)
        {
            if (key->action == ASGE::KEYS::KEY_RELEASED)
            {
                player_one.move(0);
            }
            else
            {
                player_one.move(1);
            }
        }
        if (two_player)
        {
            if (key->key == ASGE::KEYS::KEY_UP)
            {
                if (key->action == ASGE::KEYS::KEY_RELEASED)
                {
                    player_two.move(0);
                }
                else
                {
                    player_two.move(-1);
                }
            }
            if (key->key == ASGE::KEYS::KEY_DOWN)
            {
                if (key->action == ASGE::KEYS::KEY_RELEASED)
                {
                    player_two.move(0);
                }
                else
                {
                    player_two.move(1);
                }
            }
        }
    }
    else if (screen_open != GAME_SCREEN &&
             key->key == ASGE::KEYS::KEY_ESCAPE &&
             key->action == ASGE::KEYS::KEY_PRESSED)
    {
        signalExit();
    }
}

void Game::buttonChangeScreen(int action, ASGE::Sprite* sprite, int no)
{
    if (action == ASGE::MOUSE::BUTTON_PRESSED)
    {
        sprite->loadTexture(ui_elements.pressed_sprite);
    }
    else
    {
        sprite->loadTexture(ui_elements.normal_sprite);
        screen_open = no;
    }
    sprite->width(BUTTON_WIDTH);
    sprite->height(BUTTON_HEIGHT);
}

void Game::buttonQuit(int action, ASGE::Sprite* sprite)
{
    if (action == ASGE::MOUSE::BUTTON_PRESSED)
    {
        sprite->loadTexture(ui_elements.pressed_sprite);
    }
    else
    {
        sprite->loadTexture(ui_elements.normal_sprite);
        signalExit();
    }
    sprite->width(BUTTON_WIDTH);
    sprite->height(BUTTON_HEIGHT);
}

void Game::buttonTwoPlayer(int action, ASGE::Sprite* sprite, ASGE::Sprite* other, bool two)
{
    if (action == ASGE::MOUSE::BUTTON_PRESSED)
    {
        sprite->loadTexture(ui_elements.pressed_sprite);
    }
    else
    {
        other->loadTexture(ui_elements.normal_sprite);
        two_player = two;
    }
    sprite->width(BUTTON_WIDTH);
    sprite->height(BUTTON_HEIGHT);
    other->width(BUTTON_WIDTH);
    other->height(BUTTON_HEIGHT);
}

void Game::buttonHardDifficulty(int action, ASGE::Sprite* sprite, ASGE::Sprite* other, bool hard_diff)
{
    if (action == ASGE::MOUSE::BUTTON_PRESSED)
    {
        sprite->loadTexture(ui_elements.pressed_sprite);
    }
    else
    {
        other->loadTexture(ui_elements.normal_sprite);
        hard = hard_diff;
    }
    sprite->width(BUTTON_WIDTH);
    sprite->height(BUTTON_HEIGHT);
    other->width(BUTTON_WIDTH);
    other->height(BUTTON_HEIGHT);
}

void Game::buttonStartGame(int action, ASGE::Sprite* sprite)
{
    if (action == ASGE::MOUSE::BUTTON_PRESSED)
    {
        sprite->loadTexture(ui_elements.pressed_sprite);
    }
    else
    {
        sprite->loadTexture(ui_elements.normal_sprite);
        resetGame();

        ui_elements.options_1_player_button.spriteComponent()->getSprite()->loadTexture(ui_elements.pressed_sprite);
        ui_elements.options_2_player_button.spriteComponent()->getSprite()->loadTexture(ui_elements.normal_sprite);
        ui_elements.options_1_player_button.spriteComponent()->getSprite()->width(BUTTON_WIDTH);
        ui_elements.options_1_player_button.spriteComponent()->getSprite()->height(BUTTON_HEIGHT);
        ui_elements.options_2_player_button.spriteComponent()->getSprite()->width(BUTTON_WIDTH);
        ui_elements.options_2_player_button.spriteComponent()->getSprite()->height(BUTTON_HEIGHT);
        two_player = false;

        ui_elements.options_easy_button.spriteComponent()->getSprite()->loadTexture(ui_elements.pressed_sprite);
        ui_elements.options_hard_button.spriteComponent()->getSprite()->loadTexture(ui_elements.normal_sprite);
        ui_elements.options_easy_button.spriteComponent()->getSprite()->width(BUTTON_WIDTH);
        ui_elements.options_easy_button.spriteComponent()->getSprite()->height(BUTTON_HEIGHT);
        ui_elements.options_hard_button.spriteComponent()->getSprite()->width(BUTTON_WIDTH);
        ui_elements.options_hard_button.spriteComponent()->getSprite()->height(BUTTON_HEIGHT);
        hard = false ;

        screen_open = OPTIONS_SCREEN;
    }
    sprite->width(BUTTON_WIDTH);
    sprite->height(BUTTON_HEIGHT);
}

void Game::buttonRestartGame(int action, ASGE::Sprite* sprite)
{
    if (action == ASGE::MOUSE::BUTTON_PRESSED)
    {
        sprite->loadTexture(ui_elements.pressed_sprite);
    }
    else
    {
        sprite->loadTexture(ui_elements.normal_sprite);
        resetGame();
        screen_open = GAME_SCREEN;
    }
    sprite->width(BUTTON_WIDTH);
    sprite->height(BUTTON_HEIGHT);
}

void Game::clickHandler(const ASGE::SharedEventData data)
{
    auto click = static_cast<const ASGE::ClickEvent*>(data.get());

    if (screen_open == MENU_SCREEN)
    {
        ASGE::Sprite* sprite = ui_elements.menu_play_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonStartGame(click->action, sprite);
            return;
        }

        sprite = ui_elements.menu_quit_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonQuit(click->action, sprite);
            return;
        }
    }
    else if (screen_open == OPTIONS_SCREEN)
    {
        ASGE::Sprite* sprite = ui_elements.options_1_player_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonTwoPlayer(click->action, sprite, ui_elements.options_2_player_button.spriteComponent()->getSprite(), false);
            return;
        }

        sprite = ui_elements.options_2_player_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonTwoPlayer(click->action, sprite, ui_elements.options_1_player_button.spriteComponent()->getSprite(), true);
            return;
        }

        sprite = ui_elements.options_easy_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonHardDifficulty(click->action, sprite, ui_elements.options_hard_button.spriteComponent()->getSprite(), false);
            return;
        }

        sprite = ui_elements.options_hard_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonHardDifficulty(click->action, sprite, ui_elements.options_easy_button.spriteComponent()->getSprite(), true);
            return;
        }

        sprite = ui_elements.options_start_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonRestartGame(click->action, sprite);
            return;
        }
    }
    else if (screen_open == GAME_OVER_SCREEN)
    {
        // Play Again, Menu, Quit
        ASGE::Sprite* sprite = ui_elements.game_end_play_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonChangeScreen(click->action, sprite, OPTIONS_SCREEN);
            return;
        }

        sprite = ui_elements.game_end_menu_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonChangeScreen(click->action, sprite, MENU_SCREEN);
            return;
        }

        sprite = ui_elements.game_end_quit_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonQuit(click->action, sprite);
            return;
        }


    }
    else if (screen_open == PAUSE_SCREEN)
    {
        ASGE::Sprite* sprite = ui_elements.pause_resume_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonChangeScreen(click->action, sprite, GAME_SCREEN);
            return;
        }

        sprite = ui_elements.pause_menu_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonChangeScreen(click->action, sprite, MENU_SCREEN);
            return;
        }

        sprite = ui_elements.pause_quit_button.spriteComponent()->getSprite();
        if (collision.isInside(sprite, click->xpos, click->ypos))
        {
            buttonQuit(click->action, sprite);
            return;
        }
    }
}

void Game::update(const ASGE::GameTime &us)
{
    if (screen_open == GAME_SCREEN && !game_over && !game_won)
    {
        // Move GameObjects
        ball.update(us.delta_time.count()/ 1000.f);
        vector2 new_dir = collision.collision(&ball, &player_one, &player_two, obstacles, OBSTACLE_NUMBER, game_height, hard);
        ball.setDirection(new_dir.x, new_dir.y);

        player_one.update(us.delta_time.count()/ 1000.f, game_height);

        if (two_player)
        {
            player_two.update(us.delta_time.count()/ 1000.f, game_height);
        }
        else
        {
            // Check if need to launch ai_ball
            if (ball.direction().x > 0)
            {
                ai_handler.reset(ball.position(), new_dir, ball.movementComponent()->speed() + 200);
                ai_handler.track(true);
            }
            else if (ball.direction().x < 0 &&
                     ai_handler.getAIBall()->position().x + ai_handler.getAIBall()->width() >= player_two.position().x)
            {
                ai_handler.track(false);
            }

            if (ai_handler.track())
            {
                // Move AI Ball
                ai_handler.update(us.delta_time.count()/ 1000.f);
                vector2 new_ai_dir = collision.collision(ai_handler.getAIBall(), &player_one, &player_two, obstacles, OBSTACLE_NUMBER, game_height, hard);
                ai_handler.getAIBall()->setDirection(new_ai_dir.x, new_ai_dir.y);

                // Move Second Paddle
                float new_y = ai_handler.moveSecondPaddle(us.delta_time.count()/ 1000.f, player_two.position(),
                                                          player_two.movementComponent()->speed(), player_two.height(), game_height);
                player_two.spriteComponent()->getSprite()->yPos(new_y);
            }
        }

        // Award points if needed
        if (ball.position().x <= -ball.width())
        {
            player_two.increaseScore(1);
            ball.reset();
        }
        else if (ball.position().x >= game_width)
        {
            player_one.increaseScore(1);
            ball.reset();
        }

        // Randomly move blocks
        if (hard)
        {
            for (int i = 0; i < OBSTACLE_NUMBER; i++)
            {
                if (us.game_time.count() >= obstacles[i].moveTimer())
                {
                    obstacles[i].moveTimer(us.game_time.count() + obstacles[i].waitTime());
                    obstacles[i].generateNewDir();
                }
                obstacles[i].update(us.delta_time.count()/ 1000.f, game_height);
            }
        }

        // Check if somebody's won
        if (player_one.playerScore() == 10 || player_two.playerScore() == 10)
        {
            screen_open = GAME_OVER_SCREEN;
        }
    }
}

void Game::renderMainMenuScreen()
{
    renderer->renderText("PING", 470, 250, 2, ASGE::COLOURS::WHITE);
    renderer->renderSprite(*ui_elements.menu_play_button.spriteComponent()->getSprite());
    renderer->renderText("Play", 495, 368, 1, ASGE::COLOURS::BLACK);
    renderer->renderSprite(*ui_elements.menu_quit_button.spriteComponent()->getSprite());
    renderer->renderText("Quit", 495, 418, 1, ASGE::COLOURS::BLACK);
}

void Game::renderOptionsScreen()
{
    renderer->renderText("OPTIONS", 437, 200, 2, ASGE::COLOURS::WHITE);

    renderer->renderSprite(*ui_elements.options_1_player_button.spriteComponent()->getSprite());
    renderer->renderText("1-Player", 355, 328, 1, ASGE::COLOURS::BLACK);
    renderer->renderSprite(*ui_elements.options_2_player_button.spriteComponent()->getSprite());
    renderer->renderText("2-Player", 575, 328, 1, ASGE::COLOURS::BLACK);
    renderer->renderSprite(*ui_elements.options_easy_button.spriteComponent()->getSprite());
    renderer->renderText("Easy", 380, 378, 1, ASGE::COLOURS::BLACK);
    renderer->renderSprite(*ui_elements.options_hard_button.spriteComponent()->getSprite());
    renderer->renderText("Hard", 600, 378, 1, ASGE::COLOURS::BLACK);
    renderer->renderSprite(*ui_elements.options_start_button.spriteComponent()->getSprite());
    renderer->renderText("Start", 485, 458, 1, ASGE::COLOURS::BLACK);
}

void Game::renderGameScreen()
{
    renderer->renderSprite(*player_one.spriteComponent()->getSprite());
    renderer->renderSprite(*player_two.spriteComponent()->getSprite());
    renderer->renderSprite(*ball.spriteComponent()->getSprite());

    std::string score_one = "Score: ";
    score_one += std::to_string(player_one.playerScore());
    renderer->renderText(score_one, 25, 738, 1.5, ASGE::COLOURS::WHITE);

    std::string score_two = "Score: ";
    score_two += std::to_string(player_two.playerScore());
    renderer->renderText(score_two, 849, 738, 1.5, ASGE::COLOURS::WHITE);

    if (hard)
    {
        for (int i = 0; i < OBSTACLE_NUMBER; i++)
        {
            renderer->renderSprite(*obstacles[i].spriteComponent()->getSprite());
        }
    }
}

void Game::renderGameOverScreen()
{
    renderer->renderText("GAME OVER", 410, 200, 2, ASGE::COLOURS::WHITE);

    std::string score_one = "Player One Score: ";
    score_one += std::to_string(player_one.playerScore());
    renderer->renderText(score_one, 360, 300, 1.5, ASGE::COLOURS::WHITE);

    std::string score_two = "Player Two Score: ";
    score_two += std::to_string(player_two.playerScore());
    renderer->renderText(score_two, 360, 400, 1.5, ASGE::COLOURS::WHITE);

    renderer->renderSprite(*ui_elements.game_end_play_button.spriteComponent()->getSprite());
    renderer->renderText("Play Again", 462, 525, 1, ASGE::COLOURS::BLACK);
    renderer->renderSprite(*ui_elements.game_end_menu_button.spriteComponent()->getSprite());
    renderer->renderText("Menu", 487, 575, 1, ASGE::COLOURS::BLACK);
    renderer->renderSprite(*ui_elements.game_end_quit_button.spriteComponent()->getSprite());
    renderer->renderText("Quit", 487, 625, 1, ASGE::COLOURS::BLACK);
}

void Game::renderPauseScreen()
{
    renderGameScreen();

    renderer->renderText("PAUSED", 450, 200, 2, ASGE::COLOURS::WHITE);

    renderer->renderSprite(*ui_elements.pause_resume_button.spriteComponent()->getSprite());
    renderer->renderText("Resume", 483, 325, 1, ASGE::COLOURS::BLACK);
    renderer->renderSprite(*ui_elements.pause_menu_button.spriteComponent()->getSprite());
    renderer->renderText("Menu", 487, 375, 1, ASGE::COLOURS::BLACK);
    renderer->renderSprite(*ui_elements.pause_quit_button.spriteComponent()->getSprite());
    renderer->renderText("Quit", 487, 425, 1, ASGE::COLOURS::BLACK);
}

void Game::render(const ASGE::GameTime &us)
{
    if (screen_open == MENU_SCREEN)
    {
        renderMainMenuScreen();
    }
    else if (screen_open == OPTIONS_SCREEN)
    {
        renderOptionsScreen();
    }
    else if (screen_open == GAME_SCREEN)
    {
        renderGameScreen();
    }
    else if (screen_open == GAME_OVER_SCREEN)
    {
        renderGameOverScreen();
    }
    else if (screen_open == PAUSE_SCREEN)
    {
        renderPauseScreen();
    }
}

void Game::resetGame()
{
    ball.reset();
    player_one.reset();
    player_two.reset();
}




