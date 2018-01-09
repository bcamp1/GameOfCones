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

        const float screenWidth = 1300;
        const float screenHeight = 1000;
        bool up = false, down = false, left = false, right = false;
        bool up2 = false, down2 = false, left2 = false, right2 = false;
        int whichBotAttached(Block block);
        void moveTo(int index, float spd, float angle);
        void rotateTo(int index, float angle);
        bool hitBot(int index);

        float getAngle (Block& block, Robot& robot);
        bool isFront (Block& block, Robot& robot);
        float getVelX(float spd, float angle);
        float getVelY(float spd, float angle);
        bool between(float angle, float min, float max);
        float simpAngle(float angle);
        void findMinMax(Robot bot, float& min_angle, float& max_angle);
        string floatToString(float f, int precision);
        string scoreString;
        int blueScore = 0, redScore = 0;

        const float turn_angle = 3.5;
        const float speed = 6;
        bool showFramerate = false;
        bool showScore = true;

        sf::Texture arena_tx;
        sf::Sprite arena;
        sf::Clock clock;
        sf::Text fpsText;
        sf::Text scoreText;

        sf::Font font;

        sf::Texture walls_tx;
        sf::Sprite walls;

        vector <Robot> bots;
        vector<Block> blocks;
};
