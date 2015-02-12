#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QDateTime>
#include <QString>


struct CalendarEvent {
    QString title;
    QString description;
    QDateTime start;
    QDateTime end;
};

#endif // CALENDAREVENT_H
