#ifndef NEXTDEPARTURELABEL_H
#define NEXTDEPARTURELABEL_H

#include <QHttp>
#include <QLabel>
#include <QString>
#include <QTimer>
#include <QUrl>
#include <QVector>

#include "departureinfo.h"


class NextDepartureLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int updateInterval
               READ getUpdateInterval
               WRITE setUpdateInterval
               STORED false)

public:
    explicit NextDepartureLabel(QWidget *parent = 0);

    int getUpdateInterval() const;
    void setUpdateInterval(int seconds);

signals:
    
public slots:
    void updateDepartures();

protected slots:
    void httpRequestFinished(int, bool);

protected:
    static const QUrl baseUrl;
    static const QString key;
    static const double chalmersLatitude;
    static const double chalmersLongitude;

    quint64 stationId;

    int stationHttpGetId;
    int departureHttpGetId;
    QHttp http;

    QTimer updateTimer;
    QVector<DepartureInfo> departures;

    void getStationId();
    void getDepartures();
};

#endif // NEXTDEPARTURELABEL_H
