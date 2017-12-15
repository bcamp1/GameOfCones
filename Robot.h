#ifndef ROBOT_H
#define ROBOT_H
#include "Entity.h"
#include <Graphics.hpp>
#include <iostream>
using namespace std;

class Robot: public Entity
{
    public:
        int team;
        int direction = 0;
        //Forward = 1
        //Backward = 2
        //None = 0
        Robot(float x, float y, int t) : Entity({100, 100}, {x, y}, "robot-red.png"), team(t) {
            sprite.setOrigin(50, 50);
            if (team == 0) {
                    fname = "robot-blue.png";
                } else {
                    fname = "robot-red.png";
                    sprite.rotate(180);
                }
        }
    float speed = 0.5;
    protected:

    private:
};

#endif // ROBOT_H
