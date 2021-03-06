#include "Game.h"
#include "Robot.h"
#include "Block.h"
#include <iostream>
#include <math.h>
#include "Collision.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream
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
    window.setFramerateLimit(60);

    //Set up FPS Text
    font.loadFromFile("RobotoMono-Medium.ttf");
    fpsText.setFont(font);
    fpsText.setCharacterSize(30);
    fpsText.setPosition({1020, 960}); 
    fpsText.setColor(sf::Color::White);

    //Set up Score Text
    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setPosition({1020, 3}); 
    scoreText.setColor(sf::Color::White);

    cout << blueScore << endl;

    while (window.isOpen())
    { 
        if (showFramerate) {
            float fps = 1 / clock.restart().asSeconds();
            fpsText.setString(floatToString(fps, 1) + " FPS");
        }
        
        if (showScore) {
            scoreText.setString("Blue: " + std::to_string(blueScore) + "\nRed: " + std::to_string(redScore));
        }

        events(window);
        loop();
        window.clear(sf::Color(34, 34, 34));
        render(window);
        window.display();
    }

}

void Game::loop()
{
    keyboard();

    //Player 1 movement
    if (right) {rotateTo(0, turn_angle);}
    if (left) {rotateTo(0, -turn_angle);}
    if (down) {moveTo(0, -speed, -bots.at(0).sprite.getRotation()); bots.at(0).direction = 2;}
    else if (up) {moveTo(0, speed, -bots.at(0).sprite.getRotation()); bots.at(0).direction = 1;}
    if (!up && !down) {
        bots.at(0).velX = 0;
        bots.at(0).velY = 0;
        bots.at(0).direction = 0;
    }

    //Player 2 movement
    if (right2) {rotateTo(1, turn_angle);}
    if (left2) {rotateTo(1, -turn_angle);}
    if (down2) {moveTo(1, -speed, -bots.at(1).sprite.getRotation()); bots.at(1).direction = 2;}
    else if (up2) {moveTo(1, speed, -bots.at(1).sprite.getRotation()); bots.at(1).direction = 1;}
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
    //P1
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {right = true;} else {right = false;}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {left = true;} else {left = false;}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {up = true;} else {up = false;}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {down = true;} else {down = false;}

    //P2
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
    if (showFramerate) {
        window.draw(fpsText);
    }
    if (showScore) {
        window.draw(scoreText);
    }
}

void Game::events(sf::RenderWindow& window) {

    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::F:
                        showFramerate = !showFramerate;
                        break;
                    case sf::Keyboard::P:
                        showScore = !showScore;
                        break;
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                }
                break;
        }
    }
}

void Game::moveTo(int index, float spd, float angle) {
    bots.at(index).sprite.move({getVelX(spd, angle), getVelY(spd, angle)});
    if (!Collision::PixelPerfectTest(bots.at(index).sprite, walls, 0)) {
        if (!hitBot(index)) {
            bots.at(index).sprite.move({-getVelX(spd, angle), -getVelY(spd, angle)});
            bots.at(index).velX = getVelX(spd, angle);
            bots.at(index).velY = getVelY(spd, angle);
        } else {
            bots.at(index).velX = -getVelX(spd, angle) / 2;
            bots.at(index).velY = -getVelY(spd, angle) / 2; 
        }
    } else {
        bots.at(index).velX = 0;
        bots.at(index).velY = 0;
    }
}

void Game::rotateTo(int index, float angle) {
    bots.at(index).sprite.rotate(angle);
    if (Collision::PixelPerfectTest(bots.at(index).sprite, walls, 0) || hitBot(index)) {
        bots.at(index).sprite.rotate(-angle);
    }
}

bool Game::hitBot(int index) {
    for(int i = 0; i < 4; i++) {
        if (i != index && Collision::BoundingBoxTest(bots.at(index).sprite, bots.at(i).sprite)) {
            return true;
        }
    }
    return false;
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

string Game::floatToString(float f, int precision) {
    stringstream stream;
    stream << fixed << setprecision(precision) << f;
    return stream.str();
}
