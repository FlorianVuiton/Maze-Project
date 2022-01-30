#ifndef PLAYER_H
#define PLAYER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <QGLWidget>
#include <iostream>
#include "math.h"
#include "QVector3D"
#include "maze.h"

using namespace std;

class Player
{
    const float POS_Y = 2;
private:
    float rotY_ = 0;
    QVector3D position_;
    Maze* maze_;
    bool isWalking_ = false;
    bool isOut_ = false;

public:
    Player();
    void setCamera();
    void rotate(float);
    float getRotY();
    bool getIsWalking() {return isWalking_;};
    void setIsWalking(bool isWalinkg) {isWalking_ = isWalinkg;};
    void walk(float);
    QVector3D getPosition();
};

#endif // PLAYER_H
