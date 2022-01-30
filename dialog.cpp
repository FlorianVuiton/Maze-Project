#include "dialog.h"
#include "ui_dialog.h"
#include "menu.h"

Dialog::Dialog(QWidget *parent, QTime* time) :QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->timeLine->setText(time->toString());
}

//Dialog::~Dialog()
//{
//    delete ui;
//}

void Dialog::on_quit_clicked()
{
    QApplication::quit();
}
