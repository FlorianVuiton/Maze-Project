#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <QLineEdit>
#include "mazewidget.h"
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class Menu; }
QT_END_NAMESPACE

class Menu : public QMainWindow
{
     Q_OBJECT

public:
    Menu(QWidget *parent = nullptr);
    virtual ~Menu();

    static int width;
    static int height;

private slots:
    void on_startButton_clicked();

private:
     Ui::Menu *ui;
     QLineEdit qLineEdit;
};

#endif // MENU_H
