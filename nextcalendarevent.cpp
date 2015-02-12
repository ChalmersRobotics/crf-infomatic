#include "nextcalendarevent.h"

#include <QDateTime>
#include <QDebug>
#include <QMap>
#include <QSslError>
#include <QVariant>


const QUrl NextCalendarEvent::baseUrl = QUrl("https://www.googleapis.com/calendar/v3");


NextCalendarEvent::NextCalendarEvent(QWidget *parent) :
    QLabel(parent),
    httpGetId(0)
{
    connect(&http, SIGNAL(requestFinished(int,bool)),
            this, SLOT(httpRequestFinished(int,bool)));
    connect(&updateTimer, SIGNAL(timeout()),
            this, SLOT(updateCalendar()));
}

int NextCalendarEvent::getUpdateInterval() const
{
    return updateTimer.interval();
}

void NextCalendarEvent::setUpdateInterval(int seconds)
{
    updateTimer.stop();
    updateTimer.setInterval(seconds * 1000);
    if (seconds != 0) {
        updateTimer.start();
    }
}

QString NextCalendarEvent::getCalendarId() const
{
    return calendarId;
}

void NextCalendarEvent::setCalendarId(const QString &id)
{
    calendarId = id;
}

QString NextCalendarEvent::getAuthKey() const
{
    return key;
}

void NextCalendarEvent::setAuthKey(const QString &key)
{
    this->key = key;
}

void NextCalendarEvent::updateCalendar()
{
    if (calendarId.isEmpty() || key.isEmpty()) {
        return;
    }

    QUrl url(baseUrl);
    url.setPath(baseUrl.path() + "/calendars/" + calendarId + "/events");
    url.setQueryDelimiters('=', '&');

    QMap<QString, QVariant> params;
    params["maxResults"] = 2;

    QMap<QString, QVariant>::const_iterator it = params.constBegin();
    while (it != params.constEnd()) {
        url.addQueryItem(it.key(), it.value().toString());
        ++it;
    }

    url.addEncodedQueryItem("timeMin", getCurrentISOTime().replace("+", "%2B").toLatin1());
    url.addQueryItem("key", key);

    http.abort();
    http.setHost(baseUrl.host(), QHttp::ConnectionModeHttps);
    httpGetId = http.get(url.toEncoded());
}

void NextCalendarEvent::httpRequestFinished(int id, bool error)
{
    if (error) {
        clear();
        setText(http.errorString());
    } else if (id == httpGetId) {
        QByteArray data = http.readAll();
        QTextStream ts(&data);
        upcomingEvents.clear();
        do {
            QString word;
            do {
                word = readQuotedWord(ts);
            } while (!ts.atEnd() && !word.contains("summary"));
            if (!ts.atEnd()) {
                word = readQuotedWord(ts);
                if (word != calendarId) {
                    CalendarEvent event;
                    event.title = word;
                    upcomingEvents.append(event);
                }
            }
        } while (!ts.atEnd());
        if (upcomingEvents.isEmpty()) {
            clear();
            setText("No upcoming events");
        } else {
            const CalendarEvent &event = upcomingEvents.at(0);
            QString text = tr("Coming events: ") + event.title;
            for (int i = 1; i < upcomingEvents.size() && i < 2; ++i) {
                const CalendarEvent &event = upcomingEvents.at(i);
                text += ", " + event.title;
            }
            setText(text);
        }
    }
}

// Workaround for https://bugreports.qt-project.org/browse/QTBUG-26161 Qt bug
QString NextCalendarEvent::getCurrentISOTime()
{
  QDateTime local = QDateTime::currentDateTime();
  QDateTime utc = local.toUTC();
  utc.setTimeSpec(Qt::LocalTime);
  int utcOffset = utc.secsTo(local);
  local.setUtcOffset(utcOffset);

  return local.toString(Qt::ISODate);
}

QString NextCalendarEvent::readQuotedWord(QTextStream &ts)
{
    QString word;
    QChar c;
    do {
        ts >> c;
    } while (!ts.atEnd() && c != QChar('"'));
    word = c;
    do {
        ts >> c;
        word.append(c);
    } while (!ts.atEnd() && c != QChar('"'));
    word.remove(QChar('\"'));

    return word;
}
