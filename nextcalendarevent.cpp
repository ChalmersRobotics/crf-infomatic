#include "nextcalendarevent.h"
#include "ui_nextcalendarevent.h"

#include <QDateTime>
#include <QDebug>
#include <QMap>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QSslError>
#include <QVariant>


const QUrl NextCalendarEvent::baseUrl = QUrl("https://www.googleapis.com/calendar/v3");


NextCalendarEvent::NextCalendarEvent(QWidget *parent) :
    QWidget(parent),
    httpGetId(0),
    ui(new Ui::NextCalendarEvent)
{
    ui->setupUi(this);
    connect(&http, SIGNAL(requestFinished(int,bool)),
            this, SLOT(httpRequestFinished(int,bool)));
    connect(&updateTimer, SIGNAL(timeout()),
            this, SLOT(updateCalendar()));
}

NextCalendarEvent::~NextCalendarEvent()
{
    delete ui;
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
    params["singleEvents"] = true;
    params["orderBy"] = "startTime";

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

void NextCalendarEvent::clear()
{
    ui->dateWidget_1->clear();
    ui->titleLabel_1->clear();
    ui->plainTextEdit_1->clear();


    ui->dateWidget_2->clear();
    ui->titleLabel_2->clear();
    ui->plainTextEdit_2->clear();
}

void NextCalendarEvent::httpRequestFinished(int id, bool error)
{
    if (error) {
        clear();
        ui->titleLabel_1->setText(http.errorString());
    } else if (id == httpGetId) {
        QByteArray data = http.readAll();
        QScriptValue sc;
        QScriptEngine engine;
        sc = engine.evaluate("(" + QString::fromUtf8(data) + ")");

        QScriptValue items = sc.property("items");
        if (items.isValid() && items.isArray()) {

            upcomingEvents.clear();
            QScriptValueIterator it(items);
            while (it.hasNext()) {
                it.next();
                CalendarEvent event;
                QScriptValue item = it.value();

                event.title = item.property("summary").toString();
                event.description = item.property("description").toString();

                QString s = item.property("start").property("dateTime").toString();
                event.start = QDateTime::fromString(s, Qt::ISODate);

                s = item.property("end").property("dateTime").toString();
                event.end = QDateTime::fromString(s, Qt::ISODate);

                upcomingEvents.append(event);
            }
        }

        if (upcomingEvents.isEmpty()) {
            clear();
            ui->titleLabel_1->setText("No upcoming events");
        } else {
            clear();
            CalendarEvent event = upcomingEvents.at(0);
            ui->titleLabel_1->setText(event.start.time().toString("H:mm") + ": " + event.title);
            ui->dateWidget_1->setDate(event.start.date());
            if (event.description.isEmpty()) {
                ui->plainTextEdit_1->appendPlainText(tr("No description available"));
            } else {
                ui->plainTextEdit_1->appendPlainText(event.description);
            }
            ui->plainTextEdit_1->moveCursor(QTextCursor::Start);
            ui->plainTextEdit_1->ensureCursorVisible();

            event = upcomingEvents.at(1);
            ui->titleLabel_2->setText(event.start.time().toString("H:mm") + ": " + event.title);
            ui->dateWidget_2->setDate(event.start.date());
            if (event.description.isEmpty()) {
                ui->plainTextEdit_2->appendPlainText(tr("No description available"));
            } else {
                ui->plainTextEdit_2->appendPlainText(event.description);
            }
            ui->plainTextEdit_2->moveCursor(QTextCursor::Start);
            ui->plainTextEdit_2->ensureCursorVisible();
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
