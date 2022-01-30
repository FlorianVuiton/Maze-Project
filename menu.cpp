#include "menu.h"
#include "ui_menu.h"
#include "QIntValidator"

Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Menu)
{
    ui->setupUi(this);

    QPixmap bkgnd(":/res/menu.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    ui->widthLine->setValidator(new QIntValidator(4, 15, this));
    ui->heightLine->setValidator(new QIntValidator(4, 15, this));
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_startButton_clicked()
{
    Menu::width = ui->widthLine->text().toInt();
    Menu::height = ui->heightLine->text().toInt();

    MazeWidget* mazeWidget = new MazeWidget(this);
    mazeWidget->show();
    hide();
}
