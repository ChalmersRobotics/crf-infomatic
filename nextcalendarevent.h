#ifndef NEXTCALENDAREVENT_H
#define NEXTCALENDAREVENT_H

#include <QHttp>
#include <QLabel>
#include <QList>
#include <QString>
#include <QTimer>
#include <QUrl>

class QSslError;
class QTextStream;

#include "calendarevent.h"

namespace Ui {
class NextCalendarEvent;
}

class NextCalendarEvent : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int updateInterval
               READ getUpdateInterval
               WRITE setUpdateInterval
               STORED false)
    Q_PROPERTY(QString calendarId
               READ getCalendarId
               WRITE setCalendarId)
    Q_PROPERTY(QString authKey
               READ getAuthKey
               WRITE setAuthKey)

public:
    explicit NextCalendarEvent(QWidget *parent = 0);
    ~NextCalendarEvent();

    int getUpdateInterval() const;
    void setUpdateInterval(int seconds);

    QString getCalendarId() const;
    void setCalendarId(const QString &id);

    QString getAuthKey() const;
    void setAuthKey(const QString &key);

signals:
    
public slots:
    void updateCalendar();
    void clear();

protected slots:
    void httpRequestFinished(int, bool);

protected:
    static const QUrl baseUrl;
    QString key;
    QString calendarId;

    QHttp http;
    int httpGetId;

    QTimer updateTimer;
    QVector<CalendarEvent> upcomingEvents;
    QString getCurrentISOTime();
    QString readQuotedWord(QTextStream &ts);

private:
    Ui::NextCalendarEvent *ui;
};

#endif // NEXTCALENDAREVENT_H
