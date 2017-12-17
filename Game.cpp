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
    arena_tx.loadFromFile("arena.png");
    arena.setTexture(arena_tx);
    walls_tx.loadFromFile("walls.png");
    walls.setTexture(walls_tx);
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Game of Cones");
    for (int i = 0; i < 8; i++) {
        blocks.push_back(Block(0, 100*i + 100, 100*i + 100));
    }
    for (int i = 0; i < 8; i++) {
        blocks.push_back(Block(1, -100*i + 800, 100*i + 100));
    }
    blocks.at(7).x = 500;
    blocks.at(7).y = 500;

    bots.push_back(Robot(939, 939, 0)); //Player1
    bots.push_back(Robot(61, 61, 1));   //Player2
    bots.push_back(Robot(820, 939, 0)); //bot1
    bots.push_back(Robot(180, 61, 1));  //bot2

    while (window.isOpen())
    {
        events(window);
        loop();
        window.clear();
        render(window);
        window.display();
    }

}

void Game::loop()
{
    //whichSide(blocks.at(7), bots.at(0));
    keyboard();
    //Player 1 movement
    if (right) {rotateTo(bots.at(0), turn_angle);}
    if (left) {rotateTo(bots.at(0), -turn_angle);}
    if (down) {moveTo(bots.at(0), -speed, -bots.at(0).sprite.getRotation()); bots.at(0).direction = 2;}
    else if (up) {moveTo(bots.at(0), speed, -bots.at(0).sprite.getRotation()); bots.at(0).direction = 1;}
    if (!up && !down) {
        bots.at(0).velX = 0;
        bots.at(0).velY = 0;
        bots.at(0).direction = 0;
    }

    //Player 2 movement
    if (right2) {rotateTo(bots.at(1), turn_angle);}
    if (left2) {rotateTo(bots.at(1), -turn_angle);}
    if (down2) {moveTo(bots.at(1), -speed, -bots.at(1).sprite.getRotation()); bots.at(1).direction = 2;}
    else if (up2) {moveTo(bots.at(1), speed, -bots.at(1).sprite.getRotation()); bots.at(1).direction = 1;}
    if (!up2 && !down2) {
        bots.at(1).velX = 0;
        bots.at(1).velY = 0;
        bots.at(1).direction = 0;
    }

    for (int i = 0; i < blocks.size(); i++) {
        int which = whichBotAttached(blocks.at(i));
        if (which != -1) {
            if (bots.at(which).direction == 2) {
                if (!isFront(blocks.at(i), bots.at(which))) {
                    blocks.at(i).velX = bots.at(which).velX;
                    blocks.at(i).velY = bots.at(which).velY;
                    blocks.at(i).sprite.setRotation(bots.at(which).sprite.getRotation());
                }
            } else {
                if (isFront(blocks.at(i), bots.at(which))) {
                    blocks.at(i).velX = bots.at(which).velX;
                    blocks.at(i).velY = bots.at(which).velY;
                    blocks.at(i).sprite.setRotation(bots.at(which).sprite.getRotation()); 
                }
            }
        } else {
            blocks.at(i).velX = 0;
            blocks.at(i).velY = 0;
        }
    }
}

void Game::keyboard() {
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
    //window.draw(walls);
    for (int i = 0; i < 4; i++) {
        bots.at(i).show(window);
    }
    for (int i = 0; i < blocks.size(); i++) {
        blocks.at(i).show(window);
    }

    blocks.at(7).show(window);
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

void Game::moveTo(Entity& entity, float spd, float angle) {
    entity.sprite.move({getVelX(spd, angle), getVelY(spd, angle)});
    if (!Collision::PixelPerfectTest(entity.sprite, walls, 0)) {
        entity.sprite.move({-getVelX(spd, angle), -getVelY(spd, angle)});
        entity.velX = getVelX(spd, angle);
        entity.velY = getVelY(spd, angle);
    } else {
        entity.velX = 0;
        entity.velY = 0;
    }
}

void Game::rotateTo(Entity& entity, float angle) {
    entity.sprite.rotate(angle);
    if (Collision::PixelPerfectTest(entity.sprite, walls, 0)) {
        entity.sprite.rotate(-angle);
    }
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

float Game::getAngle(Block& block, Robot& robot) {
    float bx = block.x, by = block.y, rx = robot.x, ry = robot.y;
    float dx = bx - rx;
    float dy = ry - by;
    float angle = atan (dx / dy) * 180/PI;
    if (by > ry) {
        return angle + 180;
    }
    return angle;
}

bool Game::isFront(Block& block, Robot& robot) {
    float min_angle, max_angle;
    float angle = getAngle(block, robot);
    findMinMax(robot, min_angle, max_angle);
    if (between(angle, min_angle, max_angle)) {
        return true;
    }
    return false;
}

float Game::getVelX(float spd, float angle) {
    angle -= 180;
    angle *= PI/180;
    return spd * sin(angle);
}

float Game::getVelY(float spd, float angle) {
    angle -= 180;
    angle *= PI/180;
    return spd * cos(angle);
}

bool Game::between(float angle, float min, float max) {
    while (angle < min) {
        angle += 360;
    }
    if (min < angle && max > angle) {
        return true;
    }
    return false;
}

float Game::simpAngle(float angle) {
    while (angle >= 360) {
        angle -= 360;
    }
    while (angle < 0) {
        angle += 360;
    }
    return angle;
}

void Game::findMinMax(Robot bot, float& min_angle, float& max_angle) {
    float angle = bot.sprite.getRotation();
    min_angle = simpAngle(angle - 45);
    max_angle = simpAngle(angle + 45);
    while (max_angle < min_angle) {
        max_angle += 360;
    }
}
