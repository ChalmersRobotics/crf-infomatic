#include <QApplication>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOverrideCursor(Qt::BlankCursor);
    Dialog w;
    w.showFullScreen();
    
    return a.exec();
}
