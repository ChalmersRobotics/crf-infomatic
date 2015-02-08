#include "dialog.h"
#include "ui_dialog.h"

#include <QTimer>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui->dailyWlanGraph, SLOT(updatePicture()));
    // Update picture every minute
    timer->start(60*1000);
}

Dialog::~Dialog()
{
    delete ui;
}
