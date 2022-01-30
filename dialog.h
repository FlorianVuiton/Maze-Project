#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent=0, QTime* time = nullptr);
    //virtual ~Dialog();

private slots:
    void on_quit_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
