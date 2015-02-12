#include "dialog.h"
#include "ui_dialog.h"

#include <QDir>
#include <QFile>
#include <QTimer>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QFile keyFile(QDir::homePath() + QDir::separator() + "googleauth.key");
    if (keyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->eventLabel->setAuthKey(keyFile.readAll().trimmed());
        ui->eventLabel->updateCalendar();
    }
}

Dialog::~Dialog()
{
    delete ui;
}
