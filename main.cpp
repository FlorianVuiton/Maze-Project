#include "mazewidget.h"
#include "menu.h"
#include <QApplication>

int Menu::width = 10;
int Menu::height = 6;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Menu menu;
    menu.show();
    return a.exec();
}
