#include "facedetection.h"

FaceDetection::FaceDetection(Player* player) : player_(player), cap(0)
{
    cout<<"width :"<<cap.get(CAP_PROP_FRAME_WIDTH)<<endl;
    cout<<"height :"<<cap.get(CAP_PROP_FRAME_HEIGHT)<<endl;
    cap.set(CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    cap.set(CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);

    if(!cap.isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;
        return;
    }

    if( !face_cascade.load("D:\\Documents\\Code\\FISE2\\Projet Interface Graphique\\Projet_Interface_Graphique\\haarcascade_frontalface_alt.xml"))
    {
        cerr<<"Error loading haarcascade"<<endl;
        return;
    }

    state = 'D';


    // Init output window
    namedWindow("WebCam",1);
}


FaceDetection::~FaceDetection() {
    delete th;
}

void FaceDetection::setMazeGl(GLWidget* gl) {
    glwidget_ = gl;

    // Create the tread
    th = new thread(&FaceDetection::updateFaceDirection, this);
};


void FaceDetection::updateFaceDirection() {
    while(true) {
        // Get frame1
        cap >> frame;
        // Mirror effect
        cv::flip(frame,frame,1);
        // Convert to gray
        Mat frameGrey;
        cv::cvtColor(frame, frameGrey, COLOR_BGR2GRAY);
        //Get the face
        Rect face = getFace(&frameGrey);


        switch (state) {
        case 'D':
            if(face.x > 0){
                faces.clear();
                state = 'C';

            }
            break;
        case 'C':
            if(face.width <= 0){
                state = 'D';
            }
            else if (faces.size() < FACE_DETECTION_CALIBRATION){
                faces.push_back(face);
                Rect faceAverage = getFaceAverage();

                rectangle(frame, faceAverage, Scalar(255, 255, 0), 2);
            }
            else {
                Rect faceRect = getFaceAverage();

                Mat faceMatTemp;
                cv::cvtColor(Mat(frame,faceRect),faceMatTemp,COLOR_BGR2GRAY);

                nozeRect.width = (faceRect.width * 0.4);
                nozeRect.height = (faceRect.height * 0.3);
                nozeRect.x = faceRect.width / 2 - nozeRect.width / 2;
                nozeRect.y = faceRect.height / 2 - nozeRect.height / 2;
                nozeMat = Mat(faceMatTemp, nozeRect);

                state = 'S';
            }
            break;
        case 'S':
            if(face.width <= 0){
                state = 'D';
            }
            else {
                drawFaceDirection(&face);
            }
            break;
        }

        // Display frame
        imshow("WebCam", frame);

        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

Rect FaceDetection::getFace(Mat* frameGrey) {
    vector<Rect> faces;

    face_cascade.detectMultiScale(*frameGrey, faces, 1.1, 4, 0, Size(60, 60));

    if (faces.size() > 0) {
        return faces[0];
    } else {
        return Rect(0, 0, 0, 0);
    }
}

Rect FaceDetection::getFaceAverage() {
    Rect faceAverage(0, 0, 0, 0);
    for (Rect f : faces) {
        faceAverage.x += f.x;
        faceAverage.y += f.y;
        faceAverage.width += f.width;
        faceAverage.height += f.height;
    }
    faceAverage.x /= (float)faces.size();
    faceAverage.y /= (float)faces.size();
    faceAverage.width /= (float)faces.size();
    faceAverage.height /= (float)faces.size();

    return faceAverage;
}

void FaceDetection::drawFaceDirection(Rect* face) {
    // Remove first element and put the last one in the stack to always frame the face

//    faces.pop_back();
//    faces.insert(faces.begin(), *face);

    Rect faceRect = getFaceAverage();

    cv::cvtColor(Mat(frame, faceRect), faceMat, COLOR_BGR2GRAY);

    int result_cols = faceMat.cols - nozeMat.cols + 1;
    int result_rows = faceMat.rows - nozeMat.rows + 1;
    resultImage.create(result_cols, result_rows, CV_32FC1);

    matchTemplate(faceMat, nozeMat, resultImage, TM_CCORR_NORMED);

    // Localize the best match with minMaxLoc
    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);

    cv::Point vect(maxLoc.x-nozeRect.x,maxLoc.y-nozeRect.y);

    cv::Point faceCenter(faceRect.x + faceRect.width / 2, faceRect.y + faceRect.height / 2);
    cv::Point p(faceCenter.x + vect.x, faceCenter.y + vect.y);
    arrowedLine(frame, faceCenter, p, Scalar(255, 255, 255), 2);

    rectangle(frame, faceRect, Scalar(0, 255, 0), 2);

    //cout << "vectX " << vect.x << "+++++++++++ vectY "<< vect.y << endl;
    if (vect.x >= LEVEL_DETECTION) {
        player_->rotate(ROTATE);
    }
    else if (vect.y >= LEVEL_DETECTION){
        player_->setIsWalking(false);
    }
    else if (vect.x <= -LEVEL_DETECTION){
        player_->rotate(-ROTATE);
    }
    else if (vect.y <= -LEVEL_DETECTION){
        player_->setIsWalking(true);
        player_->walk(WALK);
    }
    else {
        player_->setIsWalking(false);
    }

    checkCollision();
}

void FaceDetection::checkCollision() {
    QVector3D position = player_->getPosition();

    for(int i=0;i<glwidget_->getWall()->size();i++)
    {
        //cout << glwidget_->getWall()[i][0] << endl;
        if(position.x()>glwidget_->getWall()->at(i).at(0)&& position.x() < glwidget_->getWall()->at(i).at(1) && position.z()>glwidget_->getWall()->at(i).at(2) && position.z()<glwidget_->getWall()->at(i).at(3))
        {
            player_->walk(-WALK);
        }
    }
}
