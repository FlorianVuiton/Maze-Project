#include "player.h"

Player::Player() {
    position_.setX(1);
    position_.setZ(1);
}

void Player::setCamera() {
    gluLookAt(position_.x(), 2, position_.z(), position_.x() + cos(rotY_), 2, position_.z() + sin(rotY_), 0, 1, 0);
}

void Player::rotate(float angleY){
    double yRot = angleY / 180 * 3.14159;
    rotY_ += yRot;
}

float Player::getRotY(){
    return rotY_;
}

void Player::walk(float distance)
{
    position_.setX(position_.x() + distance * cos(rotY_));
    position_.setZ(position_.z() +  distance * sin(rotY_));

    isWalking_ = true;

}

QVector3D Player::getPosition(){
    return position_;
}
