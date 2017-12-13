#include <SFML/Graphics.hpp>
#include "Robot.h"
#include "Block.h"
#pragma once
using namespace std;

class Game
{
    public:
        Game();
     private:
        void render(sf::RenderWindow & window);
        void events(sf::RenderWindow & window);
        void keyboard();
        void loop();
        void rotateBot(Robot& bot, float angle);
        const float screenWidth = 1000;
        const float screenHeight = 1000;
        bool up = false, down = false, left = false, right = false;
        bool up2 = false, down2 = false, left2 = false, right2 = false;
        void moveToward(Entity& entity, float angle);
        void moveAway(Entity& entity, float angle);

        const float turn_angle = 1.25;
        const float speed = 2.5;

        sf::Texture tx;
        sf::Sprite arena;

        Robot player1, player2, bot1, bot2;
        vector<Block> blocks;
};
