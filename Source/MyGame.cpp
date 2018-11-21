
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

#define BOUNDARY 85

#define NOT_HIT 0
#define HIT_LEFT_WALL 1
#define HIT_RIGHT_WALL 2
#define HIT_TB 3
#define HIT_LEFT_PADDLE 4
#define HIT_RIGHT_PADDLE 5

// BUGS
// TODO: Make paddle movement smoother
// TODO: Load font into game

// MyGame constructor
// Sets the game dimensions and name
MyGame::MyGame()
{
    game_width = 1024;
    game_height = 768;
    game_name = "PING";
}

// Destructor of MyGame Class
// Unregisters the key and mouse callback and empties all the sprites
MyGame::~MyGame()
{
    this->inputs->unregisterCallback(key_callback_id);
    this->inputs->unregisterCallback(mouse_callback_id);

    if (background)
    {
        delete background;
        background = nullptr;
    }
    if (foreground)
    {
        delete foreground;
        foreground = nullptr;
    }
    if (main_title)
    {
        delete main_title;
        main_title = nullptr;
    }
    if (controls_title)
    {
        delete controls_title;
        controls_title = nullptr;
    }
    if (game_over_title)
    {
        delete game_over_title;
        game_over_title = nullptr;
    }
    if (quit_title)
    {
        delete quit_title;
        quit_title = nullptr;
    }
    if (ball_sprite)
    {
        delete ball_sprite;
        ball_sprite = nullptr;
    }
    if (paddle_one)
    {
        delete paddle_one;
        paddle_one = nullptr;
    }
    if (paddle_two)
    {
        delete paddle_two;
        paddle_two = nullptr;
    }
    if (arrow_one)
    {
        delete arrow_one;
        arrow_one = nullptr;
    }
    if (arrow_two)
    {
        delete arrow_two;
        arrow_two = nullptr;
    }
    if (arrow_three)
    {
        delete arrow_three;
        arrow_three = nullptr;
    }
    if (arrow_four)
    {
        delete arrow_four;
        arrow_four = nullptr;
    }
    if (player_one_controls)
    {
        delete player_one_controls;
        player_one_controls = nullptr;
    }
    if (player_two_controls)
    {
        delete player_two_controls;
        player_two_controls = nullptr;
    }
}

// Initialises the game
// Initialises the API and loads all of the sprites
bool MyGame::init()
{
    // Return false if API wasn't initialised correctly
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

    // API and Game was initialised correctly
    return true;
}

// Resets the game back to it's original values (score, posisions, etc...)
// so a new game can be played
void MyGame::resetGame()
{
    ball.reset();
    player_one.reset();
    player_two.reset();
}

// Detects if the ball has hit anything
// Returns an integer representing what the ball has hit
int MyGame::collisionDetection()
{
    float ball_size = ball.ballSize();

    float ball_top_y = ball.yPos();
    float ball_bottom_y = ball_top_y + ball_size;

    float ball_left_x = ball.xPos();
    float ball_right_x = ball_left_x + ball_size;

    if (ball_left_x <= -ball_size)
    {
        return HIT_LEFT_WALL;
    }
    else if (ball_right_x >= game_width + ball_size)
    {
        return HIT_RIGHT_WALL;
    }
    else if (ball_top_y <= BOUNDARY ||
             ball_bottom_y >= (game_height - BOUNDARY))
    {
        return HIT_TB;
    }
    // If left coordinates of the ball are within the x and y of the paddle
    //      IF x <= player_x + player_width
    //      IF top_y >= player_y and top_y <= player_y + player_height
    //      OR bottom_y >= player_y and bottom_y <= player_y + player_height
    else if (ball_left_x <= (player_one.xPos() + player_one.paddleWidth()) && 
            ball_left_x >= player_one.xPos() + (player_one.paddleWidth() - 1) &&
            ((ball_top_y >= player_one.yPos() &&
              ball_top_y <= (player_one.yPos() + player_one.paddleHeight())) ||
             (ball_bottom_y >= player_one.yPos() &&
             ball_bottom_y <= (player_one.yPos() + player_one.paddleHeight()))))
    {
        return HIT_LEFT_PADDLE;
    }
    else if (ball_right_x >= player_two.xPos() &&
            ball_right_x <= player_two.xPos() + 1 &&
            ((ball_top_y >= player_two.yPos() &&
            ball_top_y <= (player_two.yPos() + player_two.paddleHeight())) ||
            (ball_bottom_y >= player_two.yPos() &&
            ball_bottom_y <= (player_two.yPos() + player_two.paddleHeight()))))
    {
        return HIT_RIGHT_PADDLE;
    }
    else
    {
        return NOT_HIT;
    }
}

// Handles all the key inputs for the game
// If in the menu will toggle between options and have the option to exit
// If in the game will handle the movement of the paddles
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
                    menu_option = 0;
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
            signalExit();
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
            std::cout << menu_option << std::endl;
            if (menu_option == 0)
            {
                std::cout << "Resuming";
                screen_open = GAME_SCREEN;
            }
            else if (menu_option == 1)
            {
                std::cout << "Main Menu-ing";
                screen_open = MENU_SCREEN;
                menu_option = 0;
                resetGame();
            }
            else if (menu_option == 2)
            {
                std::cout << "Quitting";
                // Exit
                signalExit();
            }
        }
    }
    else //screen_open == GAME_SCREEN
    {
        if (key->key == ASGE::KEYS::KEY_W && player_one.yPos() > BOUNDARY)
        {
            player_one.moveUp();
        }
        if (key->key == ASGE::KEYS::KEY_S &&
            player_one.yPos() + player_one.paddleHeight() <
            game_height - BOUNDARY)
        {
            player_one.moveDown();
        }
        if (key->key == ASGE::KEYS::KEY_UP && player_two.yPos() > BOUNDARY)
        {
            player_two.moveUp();
        }
        if (key->key == ASGE::KEYS::KEY_DOWN &&
            player_two.yPos() + player_two.paddleHeight() <
            game_height - BOUNDARY)
        {
            player_two.moveDown();
        }
        if (key->key == ASGE::KEYS::KEY_ESCAPE)
        {
            screen_open = QUIT_SCREEN;
            menu_option = 0;
        }
    }
}

// Updates the game
// Moves the ball and updates the positions of the sprite with the positions
// of the classes
// TODO: Simplify by having the sprites in the classes?
// Runs the collision detection and acts based on what the ball has hit
void MyGame::update(const ASGE::GameTime &us)
{
    if (screen_open == GAME_SCREEN)
    {
        // Move Ball
        float newX = ball.xPos() + (ball.ballSpeed() * (us.delta_time.count()/ 1000.f));
        ball.xPos(newX);
        ball_sprite->xPos(newX);

        float newY = ball.yPos() + (ball.ballSpeed() * (us.delta_time.count()/ 1000.f));
        ball.yPos(newY);
        ball_sprite->yPos(newY);

        // Move Player One
        paddle_one->xPos(player_one.xPos());
        paddle_one->yPos(player_one.yPos());

        // Move Player Two
        paddle_two->xPos(player_two.xPos());
        paddle_two->yPos(player_two.yPos());

        int hit = collisionDetection();
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
        if (hit == HIT_LEFT_PADDLE)
        {
            ball.multiplyVector(-1, 1);
        }
        if (hit == HIT_RIGHT_PADDLE)
        {
            ball.multiplyVector(-1, 1);
        }

        if (player_one.playerScore() == 10 || player_two.playerScore() == 10)
        {
            screen_open = GAME_OVER_SCREEN;
        }
    }
}

// Renders the sprites and text dependant on which screen the user is on
// Renders text in the menus based on the current option to show the user which
// option they have selected so far
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

        renderer->renderText("Press ENTER to start the game",
                             262, 600, 1.5, ASGE::COLOURS::WHITE);
        renderer->renderText("Press ESC to quit the game",
                             280, 675, 1.5, ASGE::COLOURS::WHITE);

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





