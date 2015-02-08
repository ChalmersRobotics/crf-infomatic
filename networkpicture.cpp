#include "networkpicture.h"

#include <QPixmap>
#include <QTemporaryFile>

NetworkPicture::NetworkPicture(QWidget *parent) :
    QLabel(parent),
    tempFile(0),
    httpGetId(0)
{
    connect(&http, SIGNAL(requestFinished(int,bool)),
            this, SLOT(httpRequestFinished(int,bool)));
}

QUrl NetworkPicture::getUrl() const
{
    return url;
}

void NetworkPicture::setUrl(const QUrl &newUrl)
{
    url = newUrl;
    updatePicture();
}

void NetworkPicture::updatePicture()
{
    if (!url.isValid()) {
        return;
    }

    // Destroy old file and create new
    delete tempFile;
    tempFile = new QTemporaryFile();
    if (!tempFile->open()) {
        return;
    }

    http.setHost(url.host(), QHttp::ConnectionModeHttp, url.port(80));
    httpGetId = http.get(url.path(), tempFile);
}

void NetworkPicture::httpRequestFinished(int id, bool error)
{
    if (id == httpGetId) {
        tempFile->close();
        QPixmap pixmap(tempFile->fileName());
        setPixmap(pixmap.scaled(this->size(), Qt::KeepAspectRatio));
    }

    if (error) {
        clear();
        setText(http.errorString());
    }
}
