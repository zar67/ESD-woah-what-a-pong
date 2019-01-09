
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

#define BOUNDARY 85.0f

// TODO: Make paddle movement smoother by using delta time
// TODO: Load font into game
// TODO: Refine AI

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

bool MyGame::init()
{
    // Return false if API wasn't initialised correctly
    if (!initAPI())
        return false;

    renderer->setSpriteMode(ASGE::SpriteSortMode::IMMEDIATE);
    inputs->use_threads = false;

    key_callback_id = inputs->addCallbackFnc(ASGE::E_KEY, &MyGame::keyHandler, this);

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

void MyGame::resetGame()
{
    ball.reset();
    player_one.reset();
    player_two.reset();
}

Vector MyGame::calculateNewDir(Player paddle, float y, float y_dir, float size, float speed)
{
    float prev_y = y_dir;
    // Calculate where the ball hit by a number from -1 to 1.
    float paddle_intersect = (paddle.yPos() + (paddle.paddleHeight() / 2)) - (y + (size / 2));
    float normalised_intersect = (paddle_intersect/(paddle.paddleHeight()/2));

    // Convert the number to a radian value.
    double bounce_angle = normalised_intersect * 1.0472; // 1.309 radians = 60 degrees.

    // Get the direction the ball should go in.
    Vector new_dir = Vector(speed*cos(bounce_angle), speed*sin(bounce_angle));

    // Ensure ball has been reflected to go in the opposite Y direction.
    if ((prev_y < 0 && y_dir > 0) || (prev_y > 0 && y_dir < 0))
    {
        new_dir.multiplyBy(1, -1);
    }

    new_dir.normalise();
    return Vector(new_dir.xPos(), new_dir.yPos());
}

Vector MyGame::intersect(Vector p1, Vector p2, Vector q1, Vector q2)
{
    // Get the equations of the two lines by getting their gradient and y-intercept.
    float p_gradient = (p2.yPos() - p1.yPos()) / (p2.xPos() - p1.xPos());
    float p_intercept = p1.yPos() - (p_gradient * p1.xPos());

    float q_gradient = (q2.yPos() - q1.yPos()) / (q2.xPos() - q1.xPos());
    float q_intercept = q1.yPos() - (q_gradient * q1.xPos());

    if (p_gradient != q_gradient)
    {
        // Get the intersect point.
        float intersect_x = (q_intercept - p_intercept) / (p_gradient - q_gradient);
        float intersect_y = p_gradient * intersect_x + p_intercept;

        return Vector(intersect_x, intersect_y);
    }
    else
    {
        return Vector(0,0);
    }
}

Vector MyGame::rayCollisionDetection(float x, float y, float x_dir, float y_dir, float size, float speed)
{
    Vector new_dir = Vector(x_dir, y_dir);
    // Find the intersection point between the ball's direction and the top boundary.
    Vector intersect_vector = intersect(
            Vector(x, y),
            Vector(x + (x_dir * 100), y + (y_dir * 100)),
            Vector(0,BOUNDARY),
            Vector(game_width,BOUNDARY));
    // If the ball intersects with the top and it hits within the screen limits and the balls position is on the top boundary, the ball has hit the top.
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) && intersect_vector.xPos() >= -size && intersect_vector.xPos() <= game_width + size && y <= intersect_vector.yPos())
    {
        new_dir.multiplyBy(1, -1);
        return new_dir;
    }

    // If ball hits bottom boundary
    intersect_vector = intersect(
            Vector(x, y),
            Vector(x + (x_dir * 100), y + (y * 100)),
            Vector(0,game_height-BOUNDARY),
            Vector(game_width,game_height-BOUNDARY));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) && intersect_vector.xPos() >= -size && intersect_vector.xPos() <= game_width + size && (y + size) >= intersect_vector.yPos())
    {
        new_dir.multiplyBy(1, -1);
        return new_dir;
    }

    // If ball hits left paddle
    intersect_vector = intersect(
            Vector(x, y),
            Vector(x + (x_dir * 100), y + (y_dir * 100)),
            Vector(player_one.xPos() + player_one.paddleWidth()+1, 0),
            Vector(player_one.xPos() + player_one.paddleWidth(), game_height));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) && intersect_vector.yPos() >= player_one.yPos() && intersect_vector.yPos() <= (player_one.yPos() + player_one.paddleHeight()) && x <= intersect_vector.xPos() && x >= player_one.xPos() && x_dir<= 0)
    {
        if (!two_player)
        {
            track_ball = true;
            ai_ball.ballSpeed(ball.ballSpeed() * 1.5f);
            ai_ball.xDir(ball.xDir());
            ai_ball.yDir(ball.yDir());
            ai_ball.ballSize(ball.ballSize());
            ai_ball.xPos(ball.xPos());
            ai_ball.yPos(ball.yPos());
        }

        new_dir = calculateNewDir(player_one, y, y_dir, size, speed);
        return new_dir;
    }
    // If ball hits right paddle
    intersect_vector = intersect(
            Vector(x + size, y),
            Vector(x + size+ (x_dir * 100), y + (y_dir * 100)),
            Vector(player_two.xPos()+1, 0),
            Vector(player_two.xPos(), game_height));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) && intersect_vector.yPos() >= player_two.yPos() && intersect_vector.yPos() <= (player_two.yPos() + player_two.paddleHeight()) && (x + size) >= intersect_vector.xPos() && (x + size) <= (intersect_vector.xPos() + player_two.paddleWidth()) && x_dir >= 0)
    {
        new_dir = calculateNewDir(player_two, y, y_dir, size, speed);
        new_dir.multiplyBy(-1, 1);
        return new_dir;
    }

    // If ball doesn't hit anything, continue with current direction
    return new_dir;
}

void MyGame::keyHandler(const ASGE::SharedEventData data)
{
    auto key = static_cast<const ASGE::KeyEvent*>(data.get());

    if (screen_open == MENU_SCREEN || screen_open == GAME_OPTION_SCREEN || screen_open == GAME_OVER_SCREEN)
    {
        if (key->key == ASGE::KEYS::KEY_UP &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            menu_option = 1 - menu_option;
        }
        if (key->key == ASGE::KEYS::KEY_DOWN &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            menu_option = 1 - menu_option;
        }
        if (key->key == ASGE::KEYS::KEY_ENTER &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            if (menu_option == 1)
            {
                if (screen_open == GAME_OPTION_SCREEN)
                {
                    screen_open = GAME_SCREEN;
                    two_player = false;
                }
                else
                {
                    signalExit();
                }
            }
            else // menu_option == 0
            {
                if (screen_open == MENU_SCREEN)
                {
                    screen_open = CONTROLS_SCREEN;
                }
                else if (screen_open == GAME_OPTION_SCREEN)
                {
                    screen_open = GAME_SCREEN;
                    two_player = true;
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
            screen_open = GAME_OPTION_SCREEN;
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
            menu_option -= 1;
        }
        if (key->key == ASGE::KEYS::KEY_DOWN &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
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
        else if (key->key == ASGE::KEYS::KEY_S && player_one.yPos() + player_one.paddleHeight() < game_height - BOUNDARY)
        {
            player_one.moveDown();
        }
        else if (two_player)
        {
            if (key->key == ASGE::KEYS::KEY_UP && player_two.yPos() > BOUNDARY)
            {
                player_two.moveUp();
            }
            else if (key->key == ASGE::KEYS::KEY_DOWN && player_two.yPos() + player_two.paddleHeight() < game_height - BOUNDARY)
            {
                player_two.moveDown();
            }
        }
        if (key->key == ASGE::KEYS::KEY_ESCAPE)
        {
            screen_open = QUIT_SCREEN;
            menu_option = 0;
        }
    }
}

void MyGame::update(const ASGE::GameTime &us)
{
    if (screen_open == GAME_SCREEN)
    {
        ball.ballSpeed(ball.ballSpeed()+0.01f);

        // Move Ball
        float new_x = ball.xPos() + (ball.ballSpeed() * ball.xDir() * (us.delta_time.count()/ 1000.f));
        ball.xPos(new_x);
        float new_y = ball.yPos() + (ball.ballSpeed() * ball.yDir() * (us.delta_time.count()/ 1000.f));
        ball.yPos(new_y);

        // Update positions
        ball.updatePosition();
        player_one.updatePosition();
        player_two.updatePosition();

        if (!two_player)
        {
            if (track_ball)
            {
                ai_ball.ballSpeed(ball.ballSpeed()*1.5f);

                // Move AI Ball
                new_x = ai_ball.xPos() + (ai_ball.ballSpeed() * ai_ball.xDir() * (us.delta_time.count()/ 1000.f));
                ai_ball.xPos(new_x);
                new_y = ai_ball.yPos() + (ai_ball.ballSpeed() * ai_ball.yDir() * (us.delta_time.count()/ 1000.f));
                ai_ball.yPos(new_y);

                Vector new_dir = rayCollisionDetection(
                        ai_ball.xPos(), ai_ball.yPos(),
                        ai_ball.xDir(), ai_ball.yDir(),
                        ai_ball.ballSize(), ai_ball.ballSpeed());
                ai_ball.yDir(new_dir.yPos());

                if (ai_ball.xPos() >= player_two.xPos() || ball.xPos() < 0)
                {
                    track_ball = false;
                }

                // Move second paddle
                float middlePos = player_two.yPos() + (player_two.paddleHeight() / 2);
                if (ai_ball.yPos() < middlePos && player_two.yPos() > BOUNDARY)
                {
                    float new_y = player_two.yPos() - (player_two.paddleSpeed() * us.delta_time.count()/1000.f);
                    player_two.yPos(new_y);
                }
                else if (ai_ball.yPos() > middlePos && player_two.yPos() + player_two.paddleHeight() < game_height - BOUNDARY)
                {
                    float new_y = player_two.yPos() + (player_two.paddleSpeed() * us.delta_time.count()/1000.f);
                    player_two.yPos(new_y);
                }
            }
        }

        // If the ball had gone off the sides of the screen, award points.
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

        Vector new_dir = rayCollisionDetection(
                ball.xPos(), ball.yPos(),
                ball.xDir(), ball.yDir(),
                ball.ballSize(), ball.ballSpeed());
        ball.xDir(new_dir.xPos());
        ball.yDir(new_dir.yPos());

        new_dir = rayCollisionDetection(
                ball.xPos(), ball.yPos()+ball.ballSize(),
                ball.xDir(), ball.yDir(),
                ball.ballSize(), ball.ballSpeed());
        ball.xDir(new_dir.xPos());

        if (player_one.playerScore() == 10 || player_two.playerScore() == 10)
        {
            screen_open = GAME_OVER_SCREEN;
        }
    }
}

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





