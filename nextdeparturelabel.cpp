#include "nextdeparturelabel.h"

#include <QDebug>
#include <QMap>
#include <QVariant>
#include <QXmlStreamReader>

#include "departureinfo.h"


const QUrl NextDepartureLabel::baseUrl = QUrl("http://api.vasttrafik.se/bin/rest.exe", QUrl::StrictMode);
const QString NextDepartureLabel::serviceName = "departureBoard";
const QString NextDepartureLabel::key = "ac6fb7fc-c5c5-482a-bc8d-35c439018468";


NextDepartureLabel::NextDepartureLabel(QWidget *parent) :
    QLabel(parent),
    stationId(9021014001960000),
    httpGetId(0)
{
    connect(&http, SIGNAL(requestFinished(int,bool)),
            this, SLOT(httpRequestFinished(int,bool)));
    connect(&updateTimer, SIGNAL(timeout()),
            this, SLOT(updateDepartures()));
    updateDepartures();
}

int NextDepartureLabel::getUpdateInterval() const
{
    return updateTimer.interval();
}

void NextDepartureLabel::setUpdateInterval(int seconds)
{
    updateTimer.stop();
    updateTimer.setInterval(seconds * 1000);
    if (seconds != 0) {
        updateTimer.start();
    }
}

void NextDepartureLabel::updateDepartures()
{
    QUrl url(baseUrl);
    url.setPath(baseUrl.path() + "/" + serviceName);
    url.setQueryDelimiters('=', '&');

    QMap<QString, QVariant> params;
    params["id"] = stationId;
    params["maxDeparturesPerLine"] = 1;

    QMap<QString, QVariant>::const_iterator it = params.constBegin();
    while (it != params.constEnd()) {
        url.addQueryItem(it.key(), it.value().toString());
        ++it;
    }

    url.addQueryItem("authKey", key);

    qDebug() << "Getting" << url;
    http.setHost(baseUrl.host(), baseUrl.port(80));
    httpGetId = http.get(url.toString());
}

void NextDepartureLabel::httpRequestFinished(int id, bool error)
{
    if (error) {
        clear();
        setText(http.errorString());
    } else if (id == httpGetId) {
        QXmlStreamReader xml;
        xml.addData(http.readAll());

        if (xml.readNextStartElement() && xml.name() == "DepartureBoard") {
            while (xml.readNextStartElement()) {
                if (xml.name() == "Departure") {
                    departures.append(DepartureInfo(xml));
                }
                xml.skipCurrentElement();
            }
        }

        if (xml.hasError()) {
            qDebug() << xml.errorString();
        } else if (!departures.isEmpty()) {
            const DepartureInfo &info = departures.at(0);
            QString text = info.time.toString("hh:mm") + " - " + info.name + " towards " + info.direction;
            for (int i = 1; i < departures.size() && i < 2; ++i) {
                const DepartureInfo &info = departures.at(i);
                text += ", " + info.time.toString("hh:mm") + " - " + info.name + " towards " + info.direction;
            }
            setText(text);
        }
    }
}
