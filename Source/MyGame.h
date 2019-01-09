
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
    Vector setNewDir(Player paddle, Ball ballObj);
    Vector rayCollisionDetection(Ball ballObj);
    Vector intersect(Vector p1, Vector p2, Vector q1, Vector q2);

private:
    void keyHandler(const ASGE::SharedEventData data);

    int  key_callback_id = -1;	        /**< Key Input Callback ID. */
    int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

    Ball ball;
    Ball aiBall;

    Player player_one = Player(50,324);
    Player player_two = Player(954,324);

    void renderMainMenu();
    void renderGameOptionsMenu();
    void renderGameScreen();
    void renderGameOver();
    void renderQuitScreen();

    //int font_index = 0;
    //ASGE::Font* game_font = nullptr;

    // Sprite Objects
    ASGE::Sprite* menu_screen = nullptr;
    ASGE::Sprite* game_options_menu = nullptr;
    ASGE::Sprite* controls_screen = nullptr;
    ASGE::Sprite* game_screen = nullptr;
    ASGE::Sprite* game_over_screen = nullptr;
    ASGE::Sprite* quit_screen = nullptr;


    int screen_open = 0;
    int menu_option = 0;

    bool twoPlayer = false;
    bool trackBall = false;

};


#endif //HELLOASGE_MYGAME_H
