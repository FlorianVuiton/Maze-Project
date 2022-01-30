#include "mazemapwidget.h"
#include "glwidget.h"

MazeMapWidget::MazeMapWidget(QWidget* parent)  : QGraphicsView(parent)
{
    graphicScene = new QGraphicsScene();
}

//void MazeMapWidget::resizeEvent(QResizeEvent *event) {
//    this->fitInView(0, 0, 300, 300, Qt::KeepAspectRatio);
//}

void MazeMapWidget::setMaze(Maze *maze){
     maze_ = maze;
     setFixedWidth(maze_->getWidth() * 30 + 50);
     setFixedHeight(maze_->getHeight() * 30 + 50);
}

void MazeMapWidget::drawMaze2D() {
    for (int i=0; i<maze_->getHeight(); i++) {
        for (int j=0; j<maze_->getWidth(); j++) {
            if (maze_->getGrid(i, 0)->isFrontier(Cell::W))
                graphicScene->addLine(0, i * 30, 0, (i+1) * 30);
            if (maze_->getGrid(i, j)->isFrontier(Cell::E))
                graphicScene->addLine((j + 1) * 30, i * 30, (j + 1) * 30, (i + 1) * 30);
            if (maze_->getGrid(i, j)->isFrontier(Cell::S))
                graphicScene->addLine(j * 30, (i + 1) * 30, (j + 1) * 30, (i + 1) * 30);
            if (maze_->getGrid(0, j)->isFrontier(Cell::N))
                graphicScene->addLine(j * 30,0 , (j + 1) * 30, 0);
        }
    }
}

void MazeMapWidget::drawPlayer(Player* player) {
    graphicScene->clear();
    QVector3D position = player->getPosition();
    float rotation = player->getRotY();

    drawMaze2D();
    graphicScene->addEllipse(position.x() * 10 - 5, position.z() *10 - 5, 10, 10, QPen(), QBrush(QColor(255,0,0)));
    graphicScene->addLine((position.x() * 10), (position.z() * 10), (position.x() * 10) + 15 * cos(rotation), position.z() * 10 + 15 * sin(rotation), QPen(QColor(255,0,0)));
    this->setScene(graphicScene);

    // Hide map if player if walking
    if (player->getIsWalking())
        this->setVisible(false);
    else
        this->setVisible(true);

}
