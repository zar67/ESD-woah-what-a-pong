#include <iostream>
#include <time.h>
#include "Source/MyGame.h"

int main()
{
    srand(time(0));

    // Game instance
    MyGame game;

    // If not initialised correctly, quit
    if (!game.init())
    {
        return -1;
    }

    // If initialised correctly, run
    game.run();
    std::cout << "Exiting Game!" << std::endl;
    return 0;
}