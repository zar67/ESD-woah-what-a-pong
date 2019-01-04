
#include <string>
#include <iostream>
#include <math.h>
#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Platform.h>
#include <Engine/Sprite.h>
#include "MyGame.h"

#define MENU_SCREEN 0
#define GAME_OPTION_SCREEN 1
#define CONTROLS_SCREEN 2
#define GAME_SCREEN 3
#define GAME_OVER_SCREEN 4
#define QUIT_SCREEN 5

#define BOUNDARY 85.0

#define NOT_HIT 0
#define HIT_LEFT_WALL 1
#define HIT_RIGHT_WALL 2
#define HIT_TB 3
#define HIT_LEFT_PADDLE 4
#define HIT_RIGHT_PADDLE 5

// TODO: Make paddle movement smoother by using delta time
// TODO: Load font into game
// TODO: Add more pong-like ball movement
// TODO: Refine AI

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

    if (game_screen)
    {
        delete game_screen;
        game_screen = nullptr;
    }
    if (menu_screen)
    {
        delete menu_screen;
        menu_screen = nullptr;
    }
    if (game_options_menu)
    {
        delete game_options_menu;
        game_options_menu = nullptr;
    }
    if (controls_screen)
    {
        delete controls_screen;
        controls_screen = nullptr;
    }
    if (game_over_screen)
    {
        delete game_over_screen;
        game_over_screen = nullptr;
    }
    if (quit_screen)
    {
        delete quit_screen;
        quit_screen = nullptr;
    }
    if (ball.getSprite())
    {
        ball.deleteSprite();
    }
    if (player_one.getSprite())
    {
        player_one.deleteSprite();
    }
    if (player_two.getSprite())
    {
        player_two.deleteSprite();
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

    // Load the Game Screen sprite
    game_screen = renderer->createRawSprite();
    game_screen->loadTexture("data/images/game_screen.png");
    game_screen->width(game_width);
    game_screen->height(game_height);

    // Load the Game Options Menu Screen sprite
    game_options_menu = renderer->createRawSprite();
    game_options_menu->loadTexture("data/images/game_options_menu.png");
    game_options_menu->width(game_width);
    game_options_menu->height(game_height);

    // Load the Main Menu Screen sprite
    menu_screen = renderer->createRawSprite();
    menu_screen->loadTexture("data/images/menu_screen.png");
    menu_screen->width(game_width);
    menu_screen->height(game_height);

    // Load the Controls Screen Sprite
    controls_screen = renderer->createRawSprite();
    controls_screen->loadTexture("data/images/controls_screen.png");
    controls_screen->width(game_width);
    controls_screen->height(game_height);

    // Load the Game Sprites
    player_one.initSprite(renderer->createRawSprite());
    player_two.initSprite(renderer->createRawSprite());
    ball.initSprite(renderer->createRawSprite());

    // Load the Game Over Screen sprite
    game_over_screen = renderer->createRawSprite();
    game_over_screen->loadTexture("data/images/game_over_screen.png");
    game_over_screen->width(game_width);
    game_over_screen->height(game_height);

    // Load the Quit Screen sprite
    quit_screen = renderer->createRawSprite();
    quit_screen->loadTexture("data/images/quit_screen.png");
    quit_screen->width(game_width);
    quit_screen->height(game_height);

    toggleFPS();

    // API and Game was initialised correctly
    return true;
}

// Resets the game back to it's original values (score, positions, etc...)
// so a new game can be played
void MyGame::resetGame()
{
    ball.reset();
    player_one.reset();
    player_two.reset();
}

Vector MyGame::intersect(Vector p1, Vector p2, Vector q1, Vector q2)
{
    float p_m = (p2.yPos() - p1.yPos()) / (p2.xPos() - p1.xPos());
    float p_c = p1.yPos() - (p_m * p1.xPos());

    float q_m = (q2.yPos() - q1.yPos()) / (q2.xPos() - q1.xPos());
    float q_c = q1.yPos() - (q_m * q1.xPos());

    if (p_m != q_m)
    {
        float intersect_x = (q_c - p_c) / (p_m - q_m);
        float intersect_y = p_m * intersect_x + p_c;

        return Vector(intersect_x, intersect_y);
    }
    else
    {
        return Vector(0,0);
    }
}

// Detects if the ball has hit anything through ray tracing
// Returns an integer values based on what had been hit
int MyGame::rayCollisionDetection()
{
    // Fine the intersection point between the ball's direction and the
    // Top boundary
    Vector intersect_vector = intersect(
            Vector(ball.xPos(), ball.yPos()), // Point one line one
            Vector(ball.xPos() + (ball.xDir() * 100), ball.yPos() + (ball.yDir() * 100)),
            Vector(0,BOUNDARY), // Point one line two
            Vector(game_width,BOUNDARY)); // Point two line two
    // If there is an intersection and the ball hits the top on within the screen
    // and the ball's position is on the top boundary
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
        intersect_vector.xPos() >= 0 && intersect_vector.xPos() <= game_width &&
        ball.yPos() <= intersect_vector.yPos())
    {
        // The ball has hit the top
        return HIT_TB;
    }

    // If ball hits bottom
    intersect_vector = intersect(
            Vector(ball.xPos(), ball.yPos()),
            Vector(ball.xPos() + (ball.xDir() * 100), ball.yPos() + (ball.yDir() * 100)),
            Vector(0,game_height-BOUNDARY),
            Vector(game_width,game_height-BOUNDARY));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
        intersect_vector.xPos() >= 0 && intersect_vector.xPos() <= game_width &&
            (ball.yPos() + ball.ballSize()) >= intersect_vector.yPos())
    {
        return HIT_TB;
    }

    // If ball intersects with left paddle
    intersect_vector = intersect(
            Vector(ball.xPos(), ball.yPos()),
            Vector(ball.xPos() + (ball.xDir() * 100), ball.yPos() + (ball.yDir() * 100)),
            Vector(player_one.xPos() + player_one.paddleWidth()+1, 0),
            Vector(player_one.xPos() + player_one.paddleWidth(), game_height));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
        intersect_vector.yPos() >= player_one.yPos() &&
        intersect_vector.yPos() <= (player_one.yPos() + player_one.paddleHeight()) &&
        ball.xPos() <= intersect_vector.xPos() &&
        ball.xDir()<= 0)
    {
        return HIT_LEFT_PADDLE;
    }
    // If ball intersects with right paddle
    intersect_vector = intersect(
            Vector(ball.xPos() + ball.ballSize(), ball.yPos()),
            Vector(ball.xPos() + ball.ballSize()+ (ball.xDir() * 100), ball.yPos() + (ball.yDir() * 100)),
            Vector(player_two.xPos()+1, 0),
            Vector(player_two.xPos(), game_height));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
        intersect_vector.yPos() >= player_two.yPos() &&
        intersect_vector.yPos() <= (player_two.yPos() + player_two.paddleHeight()) &&
        (ball.xPos() + ball.ballSize()) >= intersect_vector.xPos() &&
        ball.xDir() >= 0)
    {
        return HIT_RIGHT_PADDLE;
    }

    // If ball intersects with top of left paddle
    intersect_vector = intersect(
            Vector(ball.xPos(), ball.yPos() + ball.ballSize()),
            Vector(ball.xPos() + (ball.xDir() * 100), ball.yPos() + ball.ballSize() + (ball.yDir() * 100)),
            Vector(player_one.xPos(), player_one.yPos()),
            Vector(player_one.xPos() + 100, player_one.yPos()));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
        intersect_vector.xPos() >= player_one.xPos() &&
        intersect_vector.xPos() <= player_one.xPos() + player_one.paddleWidth() &&
        ball.yPos() >= player_one.yPos())
    {
        return HIT_TB;
    }

    // If ball intersects with top of right paddle
    intersect_vector = intersect(
            Vector(ball.xPos(), ball.yPos() + ball.ballSize()),
            Vector(ball.xPos() + (ball.xDir() * 100), ball.yPos() + ball.ballSize() + (ball.yDir() * 100)),
            Vector(player_two.xPos(), player_two.yPos()),
            Vector(player_two.xPos() + 100, player_two.yPos()));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
        intersect_vector.xPos() >= player_two.xPos() &&
        intersect_vector.xPos() <= player_two.xPos() + player_two.paddleWidth() &&
        ball.yPos() >= player_two.yPos())
    {
        return HIT_TB;
    }

    // If ball intersects with bottom of left paddle
    intersect_vector = intersect(
            Vector(ball.xPos(), ball.yPos() + ball.ballSize()),
            Vector(ball.xPos() + (ball.xDir() * 100), ball.yPos() + ball.ballSize() + (ball.yDir() * 100)),
            Vector(player_one.xPos(), player_one.yPos() + player_one.paddleHeight()),
            Vector(player_one.xPos() + 100, player_one.yPos() + player_one.paddleHeight()));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
        intersect_vector.xPos() >= player_one.xPos() &&
        intersect_vector.xPos() <= player_one.xPos() + player_one.paddleWidth() &&
        ball.yPos() <= player_one.yPos())
    {
        return HIT_TB;
    }

    // If ball intersects with bottom of right paddle
    intersect_vector = intersect(
            Vector(ball.xPos(), ball.yPos() + ball.ballSize()),
            Vector(ball.xPos() + (ball.xDir() * 100), ball.yPos() + ball.ballSize() + (ball.yDir() * 100)),
            Vector(player_two.xPos(), player_two.yPos() + player_two.paddleHeight()),
            Vector(player_two.xPos() + 100, player_two.yPos() + player_two.paddleHeight()));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
        intersect_vector.xPos() >= player_two.xPos() &&
        intersect_vector.xPos() <= player_two.xPos() + player_two.paddleWidth() &&
        ball.yPos() <= player_two.yPos())
    {
        return HIT_TB;
    }

    return NOT_HIT;
}

// Handles all the key inputs for the game
// If in the menu will toggle between options and have the option to exit
// If in the game will handle the movement of the paddles
void MyGame::keyHandler(const ASGE::SharedEventData data)
{
    auto key = static_cast<const ASGE::KeyEvent*>(data.get());

    if (screen_open == MENU_SCREEN || screen_open == GAME_OPTION_SCREEN || screen_open == GAME_OVER_SCREEN)
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
                if (screen_open == GAME_OPTION_SCREEN)
                {
                    screen_open = CONTROLS_SCREEN;
                    twoPlayer = false;
                }
                else
                {
                    // Exit
                    signalExit();
                }
            }
            else // menu_option == 0
            {
                if (screen_open == MENU_SCREEN)
                {
                    screen_open = GAME_OPTION_SCREEN;
                }
                else if (screen_open == GAME_OPTION_SCREEN)
                {
                    screen_open = CONTROLS_SCREEN;
                    twoPlayer = true;
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
            if (menu_option == 0)
            {
                screen_open = GAME_SCREEN;
            }
            else if (menu_option == 1)
            {
                screen_open = MENU_SCREEN;
                menu_option = 0;
                resetGame();
            }
            else if (menu_option == 2)
            {
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
        else if (key->key == ASGE::KEYS::KEY_S &&
            player_one.yPos() + player_one.paddleHeight() < game_height - BOUNDARY)
        {
            player_one.moveDown();
        }
        else if (twoPlayer)
        {
            if (key->key == ASGE::KEYS::KEY_UP && player_two.yPos() > BOUNDARY)
            {
                player_two.moveUp();
            }
            else if (key->key == ASGE::KEYS::KEY_DOWN &&
                player_two.yPos() + player_two.paddleHeight() < game_height - BOUNDARY)
            {
                player_two.moveDown();
            }
        }
        else if (key->key == ASGE::KEYS::KEY_ESCAPE)
        {
            screen_open = QUIT_SCREEN;
            menu_option = 0;
        }
    }
}

// Updates the game
// Moves the ball and updates the positions of the sprite with the positions
// of the classes
// Runs the collision detection and acts based on what the ball has hit
void MyGame::update(const ASGE::GameTime &us)
{
    if (screen_open == GAME_SCREEN)
    {
        ball.ballSpeed(ball.ballSpeed()+0.005);
        // Move Ball
        float newX = ball.xPos() + (ball.ballSpeed() * ball.xDir() * (us.delta_time.count()/ 1000.f));
        ball.xPos(newX);

        float newY = ball.yPos() + (ball.ballSpeed() * ball.yDir() * (us.delta_time.count()/ 1000.f));
        ball.yPos(newY);

        if (aiBall.xPos() <= player_one.xPos())
        {
            trackBall = false;
            aiBall.reset();
        }

        if (trackBall)
        {
            aiBall.ballSpeed(ball.ballSpeed());
            // Move AI Ball
            newX = aiBall.xPos() + (aiBall.ballSpeed() * aiBall.xDir() * (us.delta_time.count()/ 1000.f));
            aiBall.xPos(newX);

            newY = aiBall.yPos() + (aiBall.ballSpeed() * aiBall.yDir() * (us.delta_time.count()/ 1000.f));
            aiBall.yPos(newY);

            if (aiBall.xPos() <= player_one.xPos())
            {
                trackBall = false;
                aiBall.reset();
            }
        }

        // Update position
        ball.updatePosition();
        //aiBall.updatePosition();
        player_one.updatePosition();
        player_two.updatePosition();

        if (!twoPlayer)
        {
            float middlePos = player_two.yPos() + (player_two.paddleHeight() / 2);
            // y -= 20 * speed
            if (aiBall.yPos() <= middlePos && player_two.yPos() > BOUNDARY)
            {
                float new_y = player_two.yPos() - (player_two.paddleSpeed() * us.delta_time.count()/1000.f);
                player_two.yPos(new_y);
            }
            else if (aiBall.yPos() > middlePos && player_two.yPos() + player_two.paddleHeight() < game_height - BOUNDARY)
            {
                float new_y = player_two.yPos() + (player_two.paddleSpeed() * us.delta_time.count()/1000.f);
                player_two.yPos(new_y);
            }
        }

        if (ball.xPos() <= -ball.ballSize())
        {
            player_two.updateScore(1);
            ball.reset();
        }
        else if (ball.xPos() >= game_width)
        {
            player_one.updateScore(1);
            ball.reset();
        }

        int hit = rayCollisionDetection();
        if (hit == HIT_TB)
        {
            ball.multiplyVector(1.0, -1.0);
            if (ball.yDir() < 0)
            {
                ball.yPos(ball.yPos()-1);
            }
            else
            {
                ball.yPos(ball.yPos()+1);
            }
        }
        else if (hit == HIT_LEFT_PADDLE)
        {
            ball.multiplyVector(-1.0, 1.0);

            // set invisible ball to the same variables as the ball but with a faster speed
            aiBall.ballSpeed(ball.ballSpeed() * 2);
            aiBall.xDir(ball.xDir());
            aiBall.yDir(ball.yDir());
            aiBall.ballSize(ball.ballSize());
            aiBall.xPos(ball.xPos());
            aiBall.yPos(ball.yPos());
            trackBall = true;
        }
        else if (hit == HIT_RIGHT_PADDLE)
        {
            ball.multiplyVector(-1.0, 1.0);
        }

        if (player_one.playerScore() == 10 || player_two.playerScore() == 10)
        {
            screen_open = GAME_OVER_SCREEN;
        }
    }
}

// Renders the main menu screen
void MyGame::renderMainMenu()
{
    renderer->renderSprite(*menu_screen);

    renderer->renderText(menu_option == 0 ? ">> Play" : "   Play",
                         425, 350, 1.5, ASGE::COLOURS::WHITE);

    renderer->renderText(menu_option == 1 ? ">> Quit" : "   Quit",
                         425, 450, 1.5, ASGE::COLOURS::WHITE);
}

void MyGame::renderGameOptionsMenu()
{
    renderer->renderSprite(*game_options_menu);

    renderer->renderText(menu_option == 0 ? ">> Player vs Player" : "   Player vs Player",
                         340, 350, 1.5, ASGE::COLOURS::WHITE);

    renderer->renderText(menu_option == 1 ? ">> Player vs Computer" : "   Player vs Computer",
                         340, 450, 1.5, ASGE::COLOURS::WHITE);
}

void MyGame::renderGameScreen()
{
    renderer->renderSprite(*game_screen);

    renderer->renderSprite(*ball.getSprite());
    renderer->renderSprite(*player_one.getSprite());
    renderer->renderSprite(*player_two.getSprite());

    std::string score_one = "Score: ";
    score_one += std::to_string(player_one.playerScore());
    renderer->renderText(score_one, 25, 738, 1.5, ASGE::COLOURS::WHITE);

    std::string score_two = "Score: ";
    score_two += std::to_string(player_two.playerScore());
    renderer->renderText(score_two, 849, 738, 1.5, ASGE::COLOURS::WHITE);
}

void MyGame::renderGameOver()
{
    renderer->renderSprite(*game_over_screen);

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

void MyGame::renderQuitScreen()
{
    renderer->renderSprite(*quit_screen);

    renderer->renderText(menu_option == 0 ? ">> Resume" : "   Resume",
                         400, 350, 1.5, ASGE::COLOURS::WHITE);
    renderer->renderText(menu_option == 1 ? ">> Main Menu" : "   Main Menu",
                         400, 450, 1.5, ASGE::COLOURS::WHITE);
    renderer->renderText(menu_option == 2 ? ">> Quit" : "   Quit",
                         400, 550, 1.5, ASGE::COLOURS::WHITE);
}

// Renders the sprites and text dependant on which screen the user is on
// Renders text in the menus based on the current option to show the user which
// option they have selected so far
void MyGame::render(const ASGE::GameTime &us)
{
    if (screen_open == MENU_SCREEN)
    {
        renderMainMenu();
    }
    else if (screen_open == GAME_OPTION_SCREEN)
    {
        renderGameOptionsMenu();
    }
    else if (screen_open == CONTROLS_SCREEN)
    {
        renderer->renderSprite(*controls_screen);
    }
    else if (screen_open == GAME_SCREEN)
    {
        renderGameScreen();
    }
    else if (screen_open == GAME_OVER_SCREEN)
    {
        renderGameOver();
    }
    else if (screen_open == QUIT_SCREEN)
    {
        renderQuitScreen();
    }
}





