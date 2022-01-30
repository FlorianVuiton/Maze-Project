#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect.hpp>

#include <cstdio>
#include <iostream>
#include <QTimer>
#include <thread>

#include "player.h"
#include "glwidget.h"

using namespace std;
using namespace cv;

class FaceDetection
{
public:
    FaceDetection(Player* player);
    ~FaceDetection();
    void updateFaceDirection();
    Rect getFace(Mat*);
    Rect getFaceAverage();
    void drawFaceDirection(Rect*);
    void checkCollision();
    void setMazeGl(GLWidget* gl);

private:
    const int FRAME_WIDTH = 640;
    const int FRAME_HEIGHT = 480;
    const unsigned int FACE_DETECTION_CALIBRATION = 20;
    const int LEVEL_DETECTION = 10;
    const float WALK = 0.1;
    const float ROTATE = 5;

    //const string states[3] ={"detection", "calibration", "stable"};
    char state;

    Player* player_;
    GLWidget* glwidget_;

    vector<Rect> faces; // To store the faces captured
    std::thread* th;
    VideoCapture cap; //Default camera
    CascadeClassifier face_cascade;

    Rect nozeRect;

    Mat frame;
    Mat resultImage;    // to store the matchTemplate result

    Mat faceMat;
    Mat nozeMat;
};

#endif // FACEDETECTION_H
