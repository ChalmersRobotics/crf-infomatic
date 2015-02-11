#ifndef NETSTREAMPICTURE_H
#define NETSTREAMPICTURE_H

#include <QByteArray>
#include <QString>

class QHttpResponseHeader;

#include "networkpicture.h"


class NetStreamPicture : public NetworkPicture
{
    Q_OBJECT
public:
    explicit NetStreamPicture(QWidget *parent = 0);
    
signals:
    
public slots:
    void updatePicture();

protected slots:
    void httpRequestFinished(int, bool);
    void httpFirstResponseHeaderReceived(const QHttpResponseHeader &);
    void httpReadyRead(const QHttpResponseHeader &);

protected:
    QString boundaryString;
    qint64 bytesLeft;
    qint64 contentLength;
    QByteArray imageBuffer;
    QByteArray headerBuffer;
    static const int minHeaderBytes;
};

#endif // NETSTREAMPICTURE_H
