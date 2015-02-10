#include "departureinfo.h"

#include <QStringRef>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QXmlStreamAttributes>


DepartureInfo::DepartureInfo(QXmlStreamReader &xml)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "Departuer");

    QXmlStreamAttributes attributes = xml.attributes();
    QXmlStreamAttributes::const_iterator it = attributes.constBegin();
    while (it != attributes.constEnd()) {
        QString value(it->value().toString());
        if (it->name() == "name") {
            name = value;
        } else if (it->name() == "sname") {
            shortName = value;
        } else if (it->name() == "type") {
            type = stringToType(value);
        } else if (it->name() == "stopid") {
            stopId = value.toULongLong();
        } else if (it->name() == "stop") {
            stop = value;
        } else if (it->name() == "time") {
            time = QTime::fromString(value, "hh':'mm");
        } else if (it->name() == "date") {
            date = QDate::fromString(value, Qt::ISODate);
        } else if (it->name() == "direction") {
            direction = value;
        } else if (it->name() == "track") {
            track = value;
        }
        ++it;
    }
}

DepartureInfo::TransportType DepartureInfo::stringToType(const QString &type)
{
    if (type == "VAS") {
        return VASTTAG;
    } else if (type == "LDT") {
        return LONG_DISTANCE_TRAIN;
    } else if (type == "REG") {
        return REGIONAL_TRAIN;
    } else if (type == "BUS") {
        return BUS;
    } else if (type == "BOAT") {
        return BOAT;
    } else if (type == "TRAM") {
        return TRAM;
    } else if (type == "TAXI") {
        return TAXI;
    }

    return UNKNOWN;
}
