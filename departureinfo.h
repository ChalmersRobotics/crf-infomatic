#ifndef DEPARTUREINFO_H
#define DEPARTUREINFO_H

#include <QDate>
#include <QString>
#include <QtGlobal>
#include <QTime>

class QXmlStreamReader;


struct DepartureInfo {
    enum TransportType {
        VASTTAG,
        LONG_DISTANCE_TRAIN,
        REGIONAL_TRAIN,
        BUS,
        BOAT,
        TRAM,
        TAXI,
        UNKNOWN
    };

    QString name;
    QString shortName;
    TransportType type;
    quint64 stopId;
    QString stop;
    QTime time;
    QDate date;
    QString direction;
    QString track;

    DepartureInfo() {}
    DepartureInfo(QXmlStreamReader &xml);

protected:
    TransportType stringToType(const QString &type);
};

#endif // DEPARTUREINFO_H
