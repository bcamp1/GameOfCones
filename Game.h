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
        int whichBotAttached(Block block);
        void moveTo(Entity& entity, float spd, float angle);
        void rotateTo(Entity& entity, float angle);

        float getAngle (Block& block, Robot& robot);
        bool isFront (Block& block, Robot& robot);

        float getVelX(float spd, float angle);
        float getVelY(float spd, float angle);
        bool between(float angle, float min, float max);
        float simpAngle(float angle);
        void findMinMax(Robot bot, float& min_angle, float& max_angle);

        const float turn_angle = 1.25;
        const float speed = 2.0;

        sf::Texture arena_tx;
        sf::Sprite arena;

        sf::Texture walls_tx;
        sf::Sprite walls;

        vector <Robot> bots;
        vector<Block> blocks;
};
