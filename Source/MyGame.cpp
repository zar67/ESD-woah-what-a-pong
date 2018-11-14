
#include <string>
#include <iostream>
#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Platform.h>
#include <Engine/Sprite.h>
#include "MyGame.h"

#define MENU_SCREEN 0
#define CONTROLS_SCREEN 1
#define GAME_SCREEN 2
#define GAME_OVER_SCREEN 3
#define QUIT_SCREEN 4

#define TOP_BOUNDARY 85
#define BOTTOM_BOUNDARY 110

#define NOT_HIT 0
#define HIT_LEFT_WALL 1
#define HIT_RIGHT_WALL 2
#define HIT_TB 3
#define HIT_LEFT_PADDLE 4
#define HIT_RIGHT_PADDLE 5

// BUGS
// TODO: Fix right paddle collision (somehow)
// TODO: Made padddle movement smoother
// TODO: Load font into game

MyGame::MyGame()
{
    game_width = 1024;
    game_height = 768;
    game_name = "PING";
}

MyGame::~MyGame()
{
    this->inputs->unregisterCallback(key_callback_id);
    this->inputs->unregisterCallback(mouse_callback_id);

    if (background)
    {
        delete background;
        background = nullptr;
    }
}

bool MyGame::init()
{
    if (!initAPI())
        return false;

    renderer->setSpriteMode(ASGE::SpriteSortMode::IMMEDIATE);
    inputs->use_threads = false;

    key_callback_id = inputs->addCallbackFnc(ASGE::E_KEY, &MyGame::keyHandler,
            this);

    // Load the Background sprite
    background = renderer->createRawSprite();
    background->loadTexture("data/images/background.png");
    background->width(1024);
    background->height(768);

    // Load the Foreground sprite
    foreground = renderer->createRawSprite();
    foreground->loadTexture("data/images/foreground.png");
    foreground->width(1024);
    foreground->height(768);

    // Load the Ball sprite
    ball_sprite = renderer->createRawSprite();
    ball_sprite->loadTexture("data/images/ball.png");
    ball_sprite->width(ball.ballSize());
    ball_sprite->height(ball.ballSize());
    ball_sprite->xPos(ball.xPos());
    ball_sprite->yPos(ball.yPos());

    // Load the Paddle (Player One) sprite
    paddle_one = renderer->createRawSprite();
    paddle_one->loadTexture("data/images/paddle.png");
    paddle_one->width(player_one.paddleWidth());
    paddle_one->height(player_one.paddleHeight());
    paddle_one->xPos(player_one.xPos());
    paddle_one->yPos(player_one.yPos());

    // Load the Paddle (Player Two) sprite
    paddle_two = renderer->createRawSprite();
    paddle_two->loadTexture("data/images/paddle.png");
    paddle_two->width(player_two.paddleWidth());
    paddle_two->height(player_two.paddleHeight());
    paddle_two->xPos(player_two.xPos());
    paddle_two->yPos(player_two.yPos());

    // Load the Main Title sprite
    main_title = renderer->createRawSprite();
    main_title->loadTexture("data/images/main_title.png");
    main_title->width(500);
    main_title->height(120);
    main_title->xPos(262);
    main_title->yPos(100);

    // Load the Controls Title sprite
    controls_title = renderer->createRawSprite();
    controls_title->loadTexture("data/images/controls_title.png");
    controls_title->width(500);
    controls_title->height(120);
    controls_title->xPos(262);
    controls_title->yPos(50);

    // Load the Game Over Title sprite
    game_over_title = renderer->createRawSprite();
    game_over_title->loadTexture("data/images/game_over_title.png");
    game_over_title->width(500);
    game_over_title->height(120);
    game_over_title->xPos(262);
    game_over_title->yPos(100);

    // Load the Quit Title sprite
    quit_title = renderer->createRawSprite();
    quit_title->loadTexture("data/images/quit_title.png");
    quit_title->width(500);
    quit_title->height(120);
    quit_title->xPos(262);
    quit_title->yPos(100);

    // Load the first Arrow sprite
    arrow_one = renderer->createRawSprite();
    arrow_one->loadTexture("data/images/arrow.png");
    arrow_one->width(48);
    arrow_one->height(120);
    arrow_one->xPos(150);
    arrow_one->yPos(234);

    // Load the second Arrow sprite
    arrow_two = renderer->createRawSprite();
    arrow_two->loadTexture("data/images/arrow.png");
    arrow_two->width(48);
    arrow_two->height(120);
    arrow_two->xPos(150);
    arrow_two->yPos(394);
    arrow_two->rotationInRadians(3.14159);

    // Load the third Arrow sprite
    arrow_three = renderer->createRawSprite();
    arrow_three->loadTexture("data/images/arrow.png");
    arrow_three->width(48);
    arrow_three->height(120);
    arrow_three->xPos(854);
    arrow_three->yPos(234);

    // Load the forth Arrow sprite
    arrow_four = renderer->createRawSprite();
    arrow_four->loadTexture("data/images/arrow.png");
    arrow_four->width(48);
    arrow_four->height(120);
    arrow_four->xPos(854);
    arrow_four->yPos(394);
    arrow_four->rotationInRadians(3.14159);

    // Load the Controls (Player One) sprite
    player_one_controls = renderer->createRawSprite();
    player_one_controls->loadTexture("data/images/player_one_controls.png");
    player_one_controls->width(112);
    player_one_controls->height(210);
    player_one_controls->xPos(248);
    player_one_controls->yPos(269);

    // Load the Controls (Player Two) sprite
    player_two_controls = renderer->createRawSprite();
    player_two_controls->loadTexture("data/images/player_two_controls.png");
    player_two_controls->width(225);
    player_two_controls->height(280);
    player_two_controls->xPos(556);
    player_two_controls->yPos(234);

    toggleFPS();
    return true;
}

void MyGame::resetGame()
{
    ball.reset();
    player_one.reset();
    player_two.reset();

}

int MyGame::collisionDetection(float x_pos, float y_pos)
{
    if (x_pos <= 0)
    {
        return HIT_LEFT_WALL;
    }
    else if (x_pos >= game_width)
    {
        return HIT_RIGHT_WALL;
    }
    else if (y_pos <= TOP_BOUNDARY || y_pos >= (game_height - BOTTOM_BOUNDARY))
    {
        return HIT_TB;
    }
    else if (x_pos <= player_one.xPos() + player_one.paddleWidth() &&
        y_pos <= player_one.yPos() + player_one.paddleHeight() &&
        y_pos >= player_one.yPos())
    {
        // Has hit player one's bat
        return HIT_LEFT_PADDLE;
    }
    else if (x_pos >= player_two.xPos() &&
        y_pos <= player_two.yPos() + player_two.paddleHeight() &&
        y_pos >= player_two.yPos())
    {
        return HIT_RIGHT_PADDLE;
    }
    else
    {
        return NOT_HIT;
    }
}

void MyGame::keyHandler(const ASGE::SharedEventData data)
{
    auto key = static_cast<const ASGE::KeyEvent*>(data.get());

    if (screen_open == MENU_SCREEN || screen_open == GAME_OVER_SCREEN)
    {
        if (key->key == ASGE::KEYS::KEY_UP &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            // Toggle menu option
            menu_option = 1 - menu_option;
        }
        if (key->key == ASGE::KEYS::KEY_DOWN &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            // Toggle menu option
            menu_option = 1 - menu_option;
        }
        if (key->key == ASGE::KEYS::KEY_ENTER &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            if (menu_option == 1)
            {
                // Exit
                signalExit();
            }
            else // menu_option == 0
            {
                if (screen_open == MENU_SCREEN)
                {
                    screen_open = CONTROLS_SCREEN;
                }
                else // screen_open == GAME_OVER_SCREEN
                {
                    screen_open = MENU_SCREEN;
                    resetGame();
                }
            }
        }

    }
    else if (screen_open == CONTROLS_SCREEN)
    {
        if (key->key == ASGE::KEYS::KEY_ENTER &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            screen_open = GAME_SCREEN;
        }
        if (key->key == ASGE::KEYS::KEY_ESCAPE)
        {
            screen_open = QUIT_SCREEN;
        }
    }
    else if (screen_open == QUIT_SCREEN)
    {
        if (key->key == ASGE::KEYS::KEY_UP &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            // Toggle menu option
            menu_option -= 1;
        }
        if (key->key == ASGE::KEYS::KEY_DOWN &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            // Toggle menu option
            menu_option += 1;
        }

        // Ensure menu_option is either 0, 1 or 2
        if (menu_option < 0)
        {
            menu_option = 2;
        }
        if (menu_option > 2)
        {
            menu_option = 0;
        }

        if (key->key == ASGE::KEYS::KEY_ENTER &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            if (menu_option == 0)
            {
                screen_open = GAME_SCREEN;
            }
            if (menu_option == 1)
            {
                screen_open = MENU_SCREEN;
                resetGame();
            }
            else // menu_option == 2
            {
                // Exit
                signalExit();
            }
        }
    }
    else //screen_open == GAME_SCREEN
    {
        if (key->key == ASGE::KEYS::KEY_W)
        {
            player_one.moveUp();
        }
        if (key->key == ASGE::KEYS::KEY_S)
        {
            player_one.moveDown();
        }
        if (key->key == ASGE::KEYS::KEY_UP)
        {
            player_two.moveUp();
        }
        if (key->key == ASGE::KEYS::KEY_DOWN)
        {
            player_two.moveDown();
        }
        if (key->key == ASGE::KEYS::KEY_ESCAPE)
        {
            screen_open = QUIT_SCREEN;
        }
    }
}

void MyGame::update(const ASGE::GameTime &us)
{
    if (screen_open == GAME_SCREEN)
    {
        // Move Ball
        ball.move();
        ball_sprite->xPos(ball.xPos());
        ball_sprite->yPos(ball.yPos());

        // Move Player One
        paddle_one->xPos(player_one.xPos());
        paddle_one->yPos(player_one.yPos());

        // Move Player Two
        paddle_two->xPos(player_two.xPos());
        paddle_two->yPos(player_two.yPos());

        int hit = collisionDetection(ball.xPos(), ball.yPos());
        if (hit == HIT_TB)
        {
            ball.multiplyVector(1, -1);
        }
        if (hit == HIT_LEFT_WALL)
        {
            player_two.updateScore(1);
            ball.reset();
        }
        if (hit == HIT_RIGHT_WALL)
        {
            player_one.updateScore(1);
            ball.reset();
        }
        if (hit == HIT_LEFT_PADDLE || hit == HIT_RIGHT_PADDLE)
        {
            ball.multiplyVector(-1, 1);
        }
    }
}

void MyGame::render(const ASGE::GameTime &us)
{
    renderer->renderSprite(*background);

    if (screen_open == MENU_SCREEN)
    {
        renderer->renderSprite(*main_title);

        renderer->renderText(menu_option == 0 ? ">> Play" : "   Play",
                             425, 350, 1.5, ASGE::COLOURS::WHITE);

        renderer->renderText(menu_option == 1 ? ">> Quit" : "   Quit",
                             425, 450, 1.5, ASGE::COLOURS::WHITE);
    }
    if (screen_open == CONTROLS_SCREEN)
    {
        renderer->renderSprite(*controls_title);
        renderer->renderSprite(*paddle_one);
        renderer->renderSprite(*paddle_two);

        renderer->renderSprite(*arrow_one);
        renderer->renderSprite(*arrow_two);
        renderer->renderSprite(*arrow_three);
        renderer->renderSprite(*arrow_four);

        renderer->renderText("Player One", 256, 200, 1.0, ASGE::COLOURS::WHITE);
        renderer->renderText("Player Two", 658, 200, 1.0, ASGE::COLOURS::WHITE);

        renderer->renderSprite(*player_one_controls);
        renderer->renderSprite(*player_two_controls);

        renderer->renderText("Press ENTER to start the game", 262, 600, 1.5, ASGE::COLOURS::WHITE);
        renderer->renderText("Press ESC to quit the game", 280, 675, 1.5, ASGE::COLOURS::WHITE);

    }
    if (screen_open == GAME_SCREEN)
    {
        renderer->renderSprite(*foreground);

        renderer->renderSprite(*ball_sprite);
        renderer->renderSprite(*paddle_one);
        renderer->renderSprite(*paddle_two);

        std::string score_one = "Score: ";
        score_one += std::to_string(player_one.playerScore());
        renderer->renderText(score_one, 25, 738, 1.5, ASGE::COLOURS::WHITE);

        std::string score_two = "Score: ";
        score_two += std::to_string(player_two.playerScore());
        renderer->renderText(score_two, 849, 738, 1.5, ASGE::COLOURS::WHITE);
    }
    if (screen_open == GAME_OVER_SCREEN)
    {
        renderer->renderSprite(*game_over_title);

        std::string score_one = "Player One Score: ";
        score_one += std::to_string(player_one.playerScore());
        renderer->renderText(score_one, 384, 300, 1.5, ASGE::COLOURS::WHITE);

        std::string score_two = "Player Two Score: ";
        score_two += std::to_string(player_two.playerScore());
        renderer->renderText(score_two, 384, 400, 1.5, ASGE::COLOURS::WHITE);

        renderer->renderText(menu_option == 0 ? ">> Main Menu" : "   Main Menu",
                             400, 550, 1.5, ASGE::COLOURS::WHITE);
        renderer->renderText(menu_option == 1 ? ">> Quit" : "   Quit",
                             400, 650, 1.5, ASGE::COLOURS::WHITE);
    }
    if (screen_open == QUIT_SCREEN)
    {
        renderer->renderSprite(*quit_title);

        renderer->renderText(menu_option == 0 ? ">> Resume" : "   Resume",
                             400, 350, 1.5, ASGE::COLOURS::WHITE);
        renderer->renderText(menu_option == 1 ? ">> Main Menu" : "   Main Menu",
                             400, 450, 1.5, ASGE::COLOURS::WHITE);
        renderer->renderText(menu_option == 2 ? ">> Quit" : "   Quit",
                             400, 550, 1.5, ASGE::COLOURS::WHITE);
    }
}





