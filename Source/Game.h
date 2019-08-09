//
// Created by Zoe on 08/08/2019.
//

#ifndef PONGCLONE_GAME_H
#define PONGCLONE_GAME_H

#include <Engine/OGLGame.h>
#include "Handlers/CollisionHandler.h"
#include "Handlers/UIHandler.h"
#include "Handlers/AIHandler.h"
#include "GameObjects/Ball.h"
#include "GameObjects/Player.h"
#include "GameObjects/Obstacle.h"

const int OBSTACLE_NUMBER = 4;

class Game : public ASGE::OGLGame
{
public:
    virtual ~Game();

    bool init() override;
    void update(const ASGE::GameTime& us) override;
    void render(const ASGE::GameTime& us) override;

    void buttonChangeScreen(int action, ASGE::Sprite* sprite, int no);
    void buttonQuit(int action, ASGE::Sprite* sprite);
    void buttonTwoPlayer(int action, ASGE::Sprite* sprite, ASGE::Sprite* other, bool two);
    void buttonHardDifficulty(int action, ASGE::Sprite* sprite, ASGE::Sprite* other, bool hard_diff);
    void buttonStartGame(int action, ASGE::Sprite* sprite);
    void buttonRestartGame(int action, ASGE::Sprite* sprite);

    void resetGame();

    void renderMainMenuScreen();
    void renderOptionsScreen();
    void renderGameScreen();
    void renderGameOverScreen();
    void renderPauseScreen();

private:
    void keyHandler(const ASGE::SharedEventData data);
    void clickHandler(const ASGE::SharedEventData data);

    int  key_callback_id = -1;	        /**< Key Input Callback ID. */
    int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

    CollisionHandler collision;
    UIHandler ui_elements;
    AIHandler ai_handler;

    Ball ball;
    Player player_one;
    Player player_two;

    Obstacle obstacles[OBSTACLE_NUMBER];
    float obstacle_timer = 0;
    const float OBSTACLE_WAIT_TIME = 1000;

    int screen_open = 0;
    int menu_option = 0;

    bool two_player = false;
    bool hard = false;
    bool game_over = false;
    bool game_won = false;
    bool track_ball = false;
};


#endif //HELLOASGE_GAME_H
