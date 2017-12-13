#include "Game.h"
#include "Robot.h"
#include "Block.h"
#include <iostream>
#include <math.h>
#include "Collision.h"
#define PI 3.14159265
using namespace std;
Game::Game()
: player1(920, 950, 0),
player2(50, 50, 1),
bot1(800, 950, 0),
bot2(200, 50, 1)
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
    if (right) {
        player1.sprite.rotate(turn_angle);
    }
    if (left) {
        player1.sprite.rotate(-turn_angle);
    }
    if (down) {
        moveAway(player1, -player1.sprite.getRotation());
    }
    else if (up) {
        moveToward(player1, -player1.sprite.getRotation());

    }

    if (!up && !down) {
        player1.velX = 0;
        player1.velY = 0;
    }

    if (right2) {
        player2.sprite.rotate(turn_angle);
    }
    if (left2) {
        player2.sprite.rotate(-turn_angle);
    }
    if (down2) {
        moveAway(player2, -player2.sprite.getRotation());
    }
    else if (up2) {
        moveToward(player2, -player2.sprite.getRotation());
    }

    if (!up2 && !down2) {
        player2.velX = 0;
        player2.velY = 0;
    }

    for (int i = 0; i < blocks.size(); i++) {
        if (Collision::PixelPerfectTest(player1.sprite, blocks.at(i).sprite)) {
            blocks.at(i).velX = player1.velX;
            blocks.at(i).velY = player1.velY;
            blocks.at(i).sprite.setRotation(player1.sprite.getRotation());

        } else if (Collision::PixelPerfectTest(player2.sprite, blocks.at(i).sprite)) {
            blocks.at(i).velX = player2.velX;
            blocks.at(i).velY = player2.velY;
            blocks.at(i).sprite.setRotation(player2.sprite.getRotation());
        } else {
            blocks.at(i).velX = 0;
            blocks.at(i).velY = 0;
        }
    }
}

void Game::keyboard() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        right = true;
    } else {
        right = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        left = true;
    } else {
        left = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        up = true;
    } else {
       up = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        down = true;
    } else {
        down = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        right2 = true;
    } else {
        right2 = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        left2 = true;
    } else {
        left2 = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        up2 = true;
    } else {
       up2 = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        down2 = true;
    } else {
        down2 = false;
    }
}

void Game::render(sf::RenderWindow& window) {
    window.draw(arena);
    //block.show(window);
    player1.show(window);
    player2.show(window);
    bot1.show(window);
    bot2.show(window);

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
