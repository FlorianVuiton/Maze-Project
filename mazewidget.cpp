#include "mazewidget.h"
#include "ui_mazewidget.h"
#include "facedetection.h"
#include "menu.h"

MazeWidget::MazeWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MazeWidget)
    , maze_(Menu::width, Menu::height)
    , player_()
    , facedetection_(&player_)
    , time(new QTime(0,0,0,0))
{
    maze_.generate();

    ui->setupUi(this);
    ui->mazeGL_->setMaze(&maze_);
    ui->mazeMap_->setMaze(&maze_);
    ui->mazeGL_->setPlayer(&player_);
    ui->mazeGL_->setMaze2D(ui->mazeMap_);
    ui->mazeGL_->grabKeyboard();
    facedetection_.setMazeGl(ui->mazeGL_);

    connect(ui->mazeGL_,SIGNAL(gameEnd()),this,SLOT(gameEnd()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(updateTimer()));
    timer->start(1000);

}

MazeWidget::~MazeWidget()
{
    delete ui;
}

void MazeWidget::gameEnd() {

    Dialog dlg(this, time);
    dlg.exec();
}

void MazeWidget::updateTimer()
{
    ui->chrono->hide();
    *time = time->addSecs(1);
    ui->chrono->setText(time->toString());
    ui->chrono->show();
};

