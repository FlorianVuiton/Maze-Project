#ifndef KEY_H
#define KEY_H

#include <GL/glu.h>
#include <QImage>
#include <QString>
#include <QGLWidget>
#include <string>
#include <iostream>
#include <QDebug>
#include "QPixmap"

using namespace  std;

class Key
{
public:
    Key(string keyName);
    virtual ~Key();
    void setPosition(float posX, float posZ);
    void display();
    float getPosX(){return posX_;};
    float getPosZ(){return posZ_;};
    void setIsCaught(bool isCaught) {isCaught_ = isCaught;};
    bool getIsCaught() {return isCaught_;};

private:
    GLUquadric *keyQuad_;
    const float RADIUS = 0.5;
    const float POS_Y = 1.5;
    GLuint TextureID { 0 };
    float posX_, posZ_;
    bool isCaught_ = false;
};

#endif // KEY_H
