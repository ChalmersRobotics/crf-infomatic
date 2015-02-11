#include "netstreampicture.h"

#include <QDebug>
#include <QFile>
#include <QHttpResponseHeader>
#include <QPixmap>
#include <QTextStream>


const int NetStreamPicture::minHeaderBytes = 100;


NetStreamPicture::NetStreamPicture(QWidget *parent) :
    NetworkPicture(parent),
    bytesLeft(0)
{
    connect(&http, SIGNAL(responseHeaderReceived(QHttpResponseHeader)),
            this, SLOT(httpFirstResponseHeaderReceived(QHttpResponseHeader)));
    connect(&http, SIGNAL(readyRead(QHttpResponseHeader)),
            this, SLOT(httpReadyRead(QHttpResponseHeader)));
    updateTimer.disconnect();
}

void NetStreamPicture::updatePicture()
{
    if (!url.isValid()) {
        return;
    }

    http.abort();
    http.setHost(url.host(), QHttp::ConnectionModeHttp, url.port(80));
    httpGetId = http.get(url.path());
}

void NetStreamPicture::httpRequestFinished(int id, bool error)
{
    if (id == httpGetId) {
        qDebug() << "Request finished, sending new";
        updatePicture();
    }

    if (error) {
        qDebug() << "http request failed:" << http.errorString();
        updatePicture();
    }
}

void NetStreamPicture::httpFirstResponseHeaderReceived(const QHttpResponseHeader &hdr)
{
    QString contentType = hdr.value("Content-Type");
    // We'll just assume that boundary is last in Content-Type
    boundaryString = contentType.section("boundary=", 1);
}

void NetStreamPicture::httpReadyRead(const QHttpResponseHeader &)
{
    if (bytesLeft > 0) {
        imageBuffer.append(http.readAll());

        if (imageBuffer.size() > contentLength) {
            headerBuffer.append(imageBuffer.mid(contentLength));
            imageBuffer.remove(contentLength, imageBuffer.size());
            bytesLeft = 0;
        } else {
            bytesLeft = contentLength - imageBuffer.size();
        }

        if (bytesLeft == 0) {
            setPixmapFromData(imageBuffer);
            imageBuffer.clear();
        }
    }

    if (bytesLeft == 0) {
        headerBuffer.append(http.readAll());
        // Eat bytes until boundary string is found
        while (headerBuffer.size() >= boundaryString.size() &&
               !headerBuffer.startsWith(boundaryString.toLatin1())) {
            headerBuffer.remove(0, 1);
        }

        // If there are still bytes left, it means we found the boundary string
        if (headerBuffer.size() >= minHeaderBytes) {
            QTextStream ts(headerBuffer, QIODevice::ReadOnly);
            QString word;
            ts >> word;
            while (!ts.atEnd() && word != "Content-Length:") {
                ts >> word;
            }
            if (ts.atEnd()) {
                qDebug() << "Something borked while looking for Content-Length of stream";
                headerBuffer.clear();
            } else {
                ts >> contentLength;

                // Remove header from buffer
                int realHeaderSize = ts.pos();
                headerBuffer.remove(0, realHeaderSize);
                while (headerBuffer.at(0) == '\r' || headerBuffer.at(0) == '\n') {
                    headerBuffer.remove(0, 1);
                }

                if (headerBuffer.size() >= contentLength) {
                    setPixmapFromData(headerBuffer);
                    headerBuffer.remove(0, contentLength);
                } else {
                    imageBuffer.clear();
                    imageBuffer.append(headerBuffer);
                    bytesLeft = contentLength - headerBuffer.size();
                    headerBuffer.clear();
                }
            }
        }
    }
}

void NetStreamPicture::setPixmapFromData(const QByteArray &data)
{
    QPixmap pixmap;
    if (pixmap.loadFromData(data)) {
        setPixmap(pixmap.scaled(this->size(), Qt::KeepAspectRatio));
    } else {
        qDebug() << "pixmap.loadFromData() failed";
    }
}
