
#ifndef HELLOASGE_MYGAME_H
#define HELLOASGE_MYGAME_H

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

private:
    void keyHandler(const ASGE::SharedEventData data);
    int collisionDetection();
    void resetGame();

    int  key_callback_id = -1;	        /**< Key Input Callback ID. */
    int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

    Ball ball;

    Player player_one = Player(50,324);
    Player player_two = Player(954,324);

    void renderMainMenu();
    void renderControls();
    void renderGameScreen();
    void renderGameOver();
    void renderQuitScreen();

    //int font_index = 0;
    //ASGE::Font* game_font = nullptr;

    // Sprite Objects
    ASGE::Sprite* background = nullptr;
    ASGE::Sprite* foreground = nullptr;
    ASGE::Sprite* main_title = nullptr;
    ASGE::Sprite* controls_title = nullptr;
    ASGE::Sprite* game_over_title = nullptr;
    ASGE::Sprite* quit_title = nullptr;
    ASGE::Sprite* ball_sprite = nullptr;
    ASGE::Sprite* paddle_one = nullptr;
    ASGE::Sprite* paddle_two = nullptr;
    ASGE::Sprite* arrow_one = nullptr;
    ASGE::Sprite* arrow_two = nullptr;
    ASGE::Sprite* arrow_three = nullptr;
    ASGE::Sprite* arrow_four = nullptr;
    ASGE::Sprite* player_one_controls = nullptr;
    ASGE::Sprite* player_two_controls = nullptr;

    int screen_open = 0;
    int menu_option = 0;
};


#endif //HELLOASGE_MYGAME_H
