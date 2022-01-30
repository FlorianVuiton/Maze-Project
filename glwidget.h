#ifndef MAZEGLWIDGET_H
#define MAZEGLWIDGET_H

#include "maze.h"
#include "key.h"
#include "mazemapwidget.h"
#include "player.h"
#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>

class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    // Timer d'animation
    QTimer m_AnimationTimer;
    Maze* maze_;
    Player* player_;

//    FaceDetection facedetection;
    MazeMapWidget* maze2D;
    vector<Key*> keys;
    vector<vector<float>> wall_;

    int width_;
    int height_;

    const float COEF_CELL = 3.0;
    const float WALL_HEIGHT = 4.0;
    const float WALL_DEPTH = 0.2;
    const float WALK = 0.1;
    const float ROTATE = 5;

    bool isChanged = true;

    void drawVerticalWall(QPoint a, QPoint b);
    void drawHorizontalWall(QPoint a, QPoint b);
    void checkCollision(bool);

public:
    GLWidget(QWidget* parent = nullptr);
    void setMaze(Maze* maze);
    void setPlayer(Player* player) {player_ = player;};
    void setMaze2D(MazeMapWidget* map) {maze2D = map;};
    void displayMaze();
    void removeWall();
    vector<vector<float>>* getWall() {return &wall_;};

protected:

    // Fonction d'initialisation
    void initializeGL();

    // Fonction de redimensionnement
    void resizeGL(int width, int height);

    // Fonction d'affichage
    void paintGL();

    // Fonction de gestion d'interactions clavier
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

signals:
    void gameEnd();

};

#endif // MAZEGLWIDGET_H
