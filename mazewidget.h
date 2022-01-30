#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QMainWindow>
#include "maze.h"
#include "player.h"
#include "facedetection.h"
#include "mazemapwidget.h"
#include "QTime"
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MazeWidget; }
QT_END_NAMESPACE

class MazeWidget : public QMainWindow
{
    Q_OBJECT

public:
    MazeWidget(QWidget *parent = nullptr);
    ~MazeWidget();

private:
    Ui::MazeWidget *ui;
    Maze maze_;
    Player player_;
    MazeMapWidget maze2D_;
    FaceDetection facedetection_;

    QTime* time;
    int myTimer_value=0;

private slots:
    void gameEnd();
    void updateTimer();
};
#endif // MAZEWIDGET_H
