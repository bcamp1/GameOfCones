#ifndef BLOCK_H
#define BLOCK_H
#include "Entity.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

class Block : public Entity
{
    public:
        sf::Texture blktx;
        int type;
        Block(int blocktype, float x, float y) : Entity({25, 25}, {x, y}, "block-blue.png"), type(blocktype) {
            sprite.setOrigin(12.5, 12.5);
            if (type == 0) {
                fname = "block-red.png";
            } else if (type == 1) {
                fname = "block-blue.png";
            } else if (type == 2) {
                fname = "cone-red.png";
            } else {
                fname = "cone-blue.png";
            }
        }

    protected:

    private:
};

#endif // BLOCK_H
