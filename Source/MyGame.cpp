
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
#define GAME_SCREEN 1
#define GAME_OVER_SCREEN 2
#define QUIT_SCREEN 3

#define PADDLE_WIDTH 10.0f
#define PADDLE_HEIGHT 120.0f

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
    renderer->setClearColour(ASGE::COLOURS::BLACK);
    inputs->use_threads = false;

    key_callback_id = inputs->addCallbackFnc(
            ASGE::E_KEY, &MyGame::keyHandler, this);

    // Load the Game Sprites
    player_one.initSprite(renderer->createRawSprite());
    player_two.initSprite(renderer->createRawSprite());
    ball.initSprite(renderer->createRawSprite());

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

Vector MyGame::calculateNewDir(Player paddle, float y, float y_dir,
                               float size, float speed)
{
    float prev_y = y_dir;
    // Calculate where the ball hit by a number from -1 to 1.
    float paddle_intersect = (paddle.yPos()+(PADDLE_HEIGHT/2)) - (y + (size/2));
    float normalised_intersect = (paddle_intersect/(PADDLE_HEIGHT/2));

    // Convert the number to a radian value. 1.309 radians = 60 degrees
    const float radians = 1.0472;
    double bounce_angle = normalised_intersect * radians;

    // Get the direction the ball should go in.
    Vector new_dir = Vector(float(speed*cos(bounce_angle)), float(speed*sin(bounce_angle)));

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
    // Get the equations of the two lines by the gradient and y-intercept.
    float p_gradient = (p2.yPos() - p1.yPos()) / (p2.xPos() - p1.xPos());
    float p_intercept = p1.yPos() - (p_gradient * p1.xPos());

    float q_gradient = (q2.yPos() - q1.yPos()) / (q2.xPos() - q1.xPos());
    float q_intercept = q1.yPos() - (q_gradient * q1.xPos());

    if (p_gradient != q_gradient)
    {
        // Get the intersect point.
        float intersect_x = (q_intercept - p_intercept) /
                            (p_gradient - q_gradient);
        float intersect_y = p_gradient * intersect_x + p_intercept;

        return Vector(intersect_x, intersect_y);
    }
    else
    {
        return Vector(0,0);
    }
}

// Feedback: Consider breaking each condition down in to it's own functions. In
// play-through the ball would often get stuck, so this would need further refinement.
Vector MyGame::rayCollisionDetection(float x, float y, float x_dir, float y_dir,
                                     float size, float speed)
{
    Vector new_dir = Vector(x_dir, y_dir);

    // Find the intersect point between the ball and the top boundary.
    Vector intersect_vector = intersect(
            Vector(x, y),
            Vector(x + (x_dir * 100), y + (y_dir * 100)),
            Vector(0,0),
            Vector(game_width,0));


    // If the ball intersects with the top and it hits within the screen limits
    // and the balls position is on the top boundary, the ball has hit the top.
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
         intersect_vector.xPos() >= -size &&
         intersect_vector.xPos() <= game_width + size &&
         y <= intersect_vector.yPos() &&
         ball.yDir() < 0)
    {
        new_dir.multiplyBy(1, -1);
        return new_dir;
    }

    // If ball hits bottom boundary
    intersect_vector = intersect(
            Vector(x, y),
            Vector(x + (x_dir * 100), y + (y * 100)),
            Vector(0,game_height),
            Vector(game_width,game_height));

    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
         intersect_vector.xPos() >= -size &&
         intersect_vector.xPos() <= game_width + size &&
         (y + size) >= intersect_vector.yPos() &&
         ball.yDir() > 0)
    {
        new_dir.multiplyBy(1, -1);
        return new_dir;
    }

    // If ball hits left paddle
    intersect_vector = intersect(
            Vector(x, y),
            Vector(x + (x_dir * 100), y + (y_dir * 100)),
            Vector(player_one.xPos() + PADDLE_WIDTH+1, 0),
            Vector(player_one.xPos() + PADDLE_WIDTH, game_height));
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
         intersect_vector.yPos() >= player_one.yPos() &&
         intersect_vector.yPos() <= (player_one.yPos() + PADDLE_HEIGHT) &&
         x <= intersect_vector.xPos() &&
         x >= player_one.xPos() && x_dir <= 0)
    {
        if (!two_player)
        {
            track_ball = true;
            ai_ball.ballSpeed(ball.ballSpeed() * 1.5f);
            ai_ball.xDir(ball.xDir());
            ai_ball.yDir(ball.yDir());
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
    if ((intersect_vector.xPos() != 0 || intersect_vector.yPos() != 0) &&
         intersect_vector.yPos() >= player_two.yPos() &&
         intersect_vector.yPos() <= (player_two.yPos() + PADDLE_HEIGHT) &&
         (x + size) >= intersect_vector.xPos() &&
         (x + size) <= (intersect_vector.xPos() + PADDLE_WIDTH) &&
         x_dir >= 0)
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

    if (screen_open == MENU_SCREEN)
    {
        if (key->key == ASGE::KEYS::KEY_UP &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            menu_option = 1 - menu_option;
        }
        else if (key->key == ASGE::KEYS::KEY_DOWN &&
                 key->action == ASGE::KEYS::KEY_RELEASED)
        {
            menu_option = 1 - menu_option;
        }
        else if (key->key == ASGE::KEYS::KEY_ENTER &&
                 key->action == ASGE::KEYS::KEY_RELEASED)
        {
            if (menu_option == 1)
            {
                screen_open = GAME_SCREEN;
                two_player = false;
            }
            else // menu_option == 0
            {
                screen_open = GAME_SCREEN;
                two_player = true;
            }
        }
    }
    else if (screen_open == QUIT_SCREEN &&
             key->key == ASGE::KEYS::KEY_ENTER &&
             key->action == ASGE::KEYS::KEY_RELEASED)
    {
        screen_open = GAME_SCREEN;
    }
    else if (screen_open == GAME_OVER_SCREEN &&
             key->key == ASGE::KEYS::KEY_ENTER &&
             key->action == ASGE::KEYS::KEY_RELEASED)
    {
        screen_open = MENU_SCREEN;
        resetGame();
    }

    // Feedback: use functions for each game state or
    // even additional callbacks. It would stop this very long list
    else if (screen_open == GAME_SCREEN)
    {
        if (key->key == ASGE::KEYS::KEY_W)
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
        if (key->key == ASGE::KEYS::KEY_S)
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
        if (key->key == ASGE::KEYS::KEY_ESCAPE &&
            key->action == ASGE::KEYS::KEY_RELEASED)
        {
            screen_open = QUIT_SCREEN;
            menu_option = 0;
        }
    }
    else if (key->key == ASGE::KEYS::KEY_ESCAPE &&
             key->action == ASGE::KEYS::KEY_RELEASED)
    {
        signalExit();
    }
}

void MyGame::update(const ASGE::GameTime &us)
{
    if (screen_open == GAME_SCREEN)
    {
        ball.ballSpeed(ball.ballSpeed()+0.01f);

        // Move Ball
        float new_x = ball.xPos() +
                float(ball.ballSpeed()*ball.xDir()*(us.delta_time.count()/ 1000.f));
        ball.xPos(new_x);

        float new_y = ball.yPos() +
                float(ball.ballSpeed()*ball.yDir()*(us.delta_time.count()/ 1000.f));
        ball.yPos(new_y);

        // Update positions
        ball.updatePosition();

        if (player_one.yPos() + PADDLE_HEIGHT > game_height)
        {
            player_one.move(0);
            player_one.yPos(game_height-1-PADDLE_HEIGHT);
        }
        else if (player_one.yPos() < 0)
        {
            player_one.move(0);
            player_one.yPos(1);
        }
        player_one.updatePosition(us.delta_time.count()/ 1000.f);

        if (two_player)
        {
            if (player_two.yPos() + PADDLE_HEIGHT > game_height)
            {
                player_two.move(0);
                player_two.yPos(game_height-1-PADDLE_HEIGHT);
            }
            else if (player_two.yPos() < 0)
            {
                player_two.move(0);
                player_two.yPos(1);
            }
        }
        else
        {
            // Feedback: Why not create an AI class and move this login in to it
            if (track_ball)
            {
                // 1.5f?
                ai_ball.ballSpeed(ball.ballSpeed()*1.5f);

                // Move AI Ball
                new_x = float(ai_ball.xPos() +
                        (ai_ball.ballSpeed() * ai_ball.xDir() *
                        (us.delta_time.count()/ 1000.f)));
                ai_ball.xPos(new_x);
                new_y = float(ai_ball.yPos() +
                        (ai_ball.ballSpeed() * ai_ball.yDir() *
                        (us.delta_time.count()/ 1000.f)));
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
                float middlePos = player_two.yPos() + (PADDLE_HEIGHT / 2);
                if (ai_ball.yPos() < middlePos && player_two.yPos() > 0)
                {
                    new_y = float(player_two.yPos() -
                            (player_two.speed() * us.delta_time.count()/1000.f));
                    player_two.yPos(new_y);
                }
                else if (ai_ball.yPos() > middlePos && player_two.yPos() + PADDLE_HEIGHT < game_height)
                {
                    new_y = float(player_two.yPos() +
                            (player_two.speed() * us.delta_time.count()/1000.f));
                    player_two.yPos(new_y);
                }
            }
        }
        player_two.updatePosition(us.delta_time.count()/ 1000.f);

        // If the ball had gone off the sides of the screen, award points.
        if (ball.xPos() <= -ball.ballSize())
        {
            player_two.increaseScore();
            ball.reset();
        }
        else if (ball.xPos() >= game_width)
        {
            player_one.increaseScore();
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
    renderer->renderText("PING",
                         470, 200, 2, ASGE::COLOURS::WHITE);

    renderer->renderText(
            menu_option == 0 ? ">> Player vs Player" : "   Player vs Player",
            340, 350, 1.5, ASGE::COLOURS::WHITE);

    renderer->renderText(
            menu_option == 1 ? ">> Player vs Computer" : "   Player vs Computer",
            320, 400, 1.5, ASGE::COLOURS::WHITE);

    renderer->renderText("Player One use W and S",
                         340, 520, 1.5, ASGE::COLOURS::WHITE);
    renderer->renderText("Player Two use Up and Down",
                         315, 550, 1.5, ASGE::COLOURS::WHITE);

    renderer->renderText("Press ESC to quit",
                         385, 600, 1.5, ASGE::COLOURS::WHITE);
}

void MyGame::renderGameScreen()
{
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
    renderer->renderText("GAME OVER",
                         410, 200, 2, ASGE::COLOURS::WHITE);

    std::string score_one = "Player One Score: ";
    score_one += std::to_string(player_one.playerScore());
    renderer->renderText(score_one, 360, 300, 1.5, ASGE::COLOURS::WHITE);

    std::string score_two = "Player Two Score: ";
    score_two += std::to_string(player_two.playerScore());
    renderer->renderText(score_two, 360, 400, 1.5, ASGE::COLOURS::WHITE);

    renderer->renderText("Press ENTER for the main menu",
                         270, 550, 1.5, ASGE::COLOURS::WHITE);
    renderer->renderText("Press ESC to quit",
                         375, 600, 1.5, ASGE::COLOURS::WHITE);
}

void MyGame::renderQuitScreen()
{
    renderer->renderText("QUIT",
                         470, 200, 2, ASGE::COLOURS::WHITE);

    renderer->renderText("Press ENTER to resume",
                         345, 350, 1.5, ASGE::COLOURS::WHITE);
    renderer->renderText("Press ESC to quit",
                         380, 450, 1.5, ASGE::COLOURS::WHITE);
}

void MyGame::render(const ASGE::GameTime &us)
{
    if (screen_open == MENU_SCREEN)
    {
        renderMainMenu();
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





