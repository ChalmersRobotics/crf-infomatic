#include <QApplication>
#include <QTime>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOverrideCursor(Qt::BlankCursor);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    Dialog w;
    w.showFullScreen();
    
    return a.exec();
}
