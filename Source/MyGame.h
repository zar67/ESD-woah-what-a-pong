
#ifndef HELLOASGE_MYGAME_H
#define HELLOASGE_MYGAME_H

#include <math.h>
#include <Engine/OGLGame.h>
#include "Ball.h"
#include "Player.h"

class MyGame : public ASGE::OGLGame
{
public:
    MyGame();
    virtual ~MyGame();

    bool init() override;
    void update(const ASGE::GameTime& us) override;
    void render(const ASGE::GameTime& us) override;

    void resetGame();

    // Used to calculate the new direction the ball should go in.
    // The angle the ball should go off at is calculated depending on where the
    // ball hits the paddle
    // Returns the new direction.
    Vector calculateNewDir(Player paddle, float y, float y_dir,
                           float size, float speed);

    // Used to see if the two lines intersect.
    // Returns the point of intersection.
    Vector intersect(Vector p1, Vector p2, Vector q1, Vector q2);

    // Uses ray casting and bounding boxes to detect if given positions have hit
    // anything.
    // The parameters are passed in instead of the ball object because both the
    // AI ball and the game ball need to be checked for collisions.
    // Returns the new direction the ball should go in.
    Vector rayCollisionDetection(float x, float y, float x_dir, float y_dir,
                                 float size, float speed);

    void renderMainMenu();
    void renderGameOptionsMenu();
    void renderGameScreen();
    void renderGameOver();
    void renderQuitScreen();

private:
    // Handles all the key inputs for the game.
    // If in the menu will toggle between options and have the option to exit.
    // If in the game will handle the movement of the paddles.
    void keyHandler(const ASGE::SharedEventData data);

    int  key_callback_id = -1;	        /**< Key Input Callback ID. */
    int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

    Ball ball;

    // Used when the ball bounces off the left paddle.
    // The AI ball moves faster and the right paddle positions itself on the
    // same Y, giving the illusion it is trying to predict the ball's
    // trajectory.
    Ball ai_ball;

    // Player one is always player controlled, player two can be either player
    // or 'computer' controlled.
    Player player_one = Player(50,324);
    Player player_two = Player(954,324);

    // Sprite Objects
    ASGE::Sprite* menu_screen = nullptr;
    ASGE::Sprite* background = nullptr;
    ASGE::Sprite* game_screen = nullptr;
    ASGE::Sprite* game_over_screen = nullptr;

    // Used to determine what screen should be rendered.
    int screen_open = 0;

    int menu_option = 0;

    // Used to show whether the second paddle is controlled by the player.
    bool two_player = false;
    // Used to show if the second paddle should follow the AI ball's position.
    bool track_ball = false;
};

#endif //HELLOASGE_MYGAME_H