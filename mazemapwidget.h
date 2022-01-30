#ifndef MAZEMAPWIDGET_H
#define MAZEMAPWIDGET_H

#include <QGraphicsView>
#include "maze.h"
#include "cell.h"
#include "player.h"



class MazeMapWidget : public QGraphicsView
{
    Q_OBJECT

private:
    QGraphicsScene* graphicScene;

    Maze* maze_;

public:
    MazeMapWidget(QWidget *parent = 0);
    //void resizeEvent(QResizeEvent *event);
    void setMaze(Maze* maze);
    void drawMaze2D();
    void drawPlayer(Player* player);
};

#endif // MAZEMAPWIDGET_H
