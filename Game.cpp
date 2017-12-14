#include "Game.h"
#include "Robot.h"
#include "Block.h"
#include <iostream>
#include <math.h>
#include "Collision.h"
#define PI 3.14159265
using namespace std;
Game::Game()
{
    tx.loadFromFile("arena.png");
    arena.setTexture(tx);
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Game of Cones");
    for (int i = 0; i < 8; i++) {
        blocks.push_back(Block(0, 100*i + 100, 100*i + 100));
    }
    for (int i = 0; i < 8; i++) {
        blocks.push_back(Block(1, -100*i + 800, 100*i + 100));
    }

    bots.push_back(Robot(920, 950, 0)); //Player1
    bots.push_back(Robot(50, 50, 1));   //Player2
    bots.push_back(Robot(800, 950, 0)); //bot1
    bots.push_back(Robot(200, 50, 1));  //bot2

    while (window.isOpen())
    {
        events(window);
        loop();
        window.clear();
        render(window);
        window.display();
    }

}

void Game::loop() {
    keyboard();
    //Player 1 movement
    if (right) {bots.at(0).sprite.rotate(turn_angle);}
    if (left) {bots.at(0).sprite.rotate(-turn_angle);}
    if (down) {moveAway(bots.at(0), -bots.at(0).sprite.getRotation());}
    else if (up) {moveToward(bots.at(0), -bots.at(0).sprite.getRotation());}
    if (!up && !down) {
        bots.at(0).velX = 0;
        bots.at(0).velY = 0;
    }

    //Player 2 movement
    if (right2) {bots.at(1).sprite.rotate(turn_angle);}
    if (left2) {bots.at(1).sprite.rotate(-turn_angle);}
    if (down2) {moveAway(bots.at(1), -bots.at(1).sprite.getRotation());}
    else if (up2) {moveToward(bots.at(1), -bots.at(1).sprite.getRotation());}
    if (!up2 && !down2) {
        bots.at(1).velX = 0;
        bots.at(1).velY = 0;
    }

    for (int i = 0; i < blocks.size(); i++) {
        int which = whichBotAttached(blocks.at(i));
        if (which != -1) {
            blocks.at(i).velX = bots.at(which).velX;
            blocks.at(i).velY = bots.at(which).velY;
            blocks.at(i).sprite.setRotation(bots.at(which).sprite.getRotation());
        } else {
            blocks.at(i).velX = 0;
            blocks.at(i).velY = 0;
        }
    }
}

void Game::keyboard() {
    /*
    Player Movement
    Player1 - WASD
    Player2 - Arrow Keys
    */
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {right = true;} else {right = false;}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {left = true;} else {left = false;}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {up = true;} else {up = false;}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {down = true;} else {down = false;}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {right2 = true;} else {right2 = false;}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {left2 = true;} else {left2 = false;}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {up2 = true;} else {up2 = false;}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {down2 = true;} else {down2 = false;}
}

void Game::render(sf::RenderWindow& window) {
    window.draw(arena);
    for (int i = 0; i < 4; i++) {
        bots.at(i).show(window);
    }
    for (int i = 0; i < blocks.size(); i++) {
        blocks.at(i).show(window);
    }
}

void Game::events(sf::RenderWindow& window) {

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::rotateBot(Robot& bot, float angle) {
    bot.sprite.setRotation(angle);
}

void Game::moveToward(Entity& entity, float angle) {
    float d = speed;
    angle -= 180;
    angle *= PI/180;
    entity.velY = d * cos(angle);
    entity.velX = d * sin(angle);
}
void Game::moveAway(Entity& entity, float angle) {
    float d = -speed;
    angle -= 180;
    angle *= PI/180;
    entity.velY = d * cos(angle);
    entity.velX = d * sin(angle);
}

//If returned -1, block isn't attached to a bot
int Game::whichBotAttached(Block block) {
    for (int i = 0; i < bots.size(); i++) {
        if (Collision::PixelPerfectTest(bots.at(i).sprite, block.sprite)) {
            return i;
        }
    }
    return -1;
}
