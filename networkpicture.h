#ifndef NETWORKPICTURE_H
#define NETWORKPICTURE_H

#include <QHttp>
#include <QUrl>
#include <QLabel>
#include <QTimer>

class QTemporaryFile;

class NetworkPicture : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ getUrl WRITE setUrl)
    Q_PROPERTY(int updateInterval
               READ getUpdateInterval
               WRITE setUpdateInterval
               STORED false)

public:
    explicit NetworkPicture(QWidget *parent = 0);
    
    QUrl getUrl() const;
    void setUrl(const QUrl &newUrl);

    int getUpdateInterval() const;
    void setUpdateInterval(int seconds);

signals:
    
public slots:
    void updatePicture();
    
protected slots:
    void httpRequestFinished(int, bool);

protected:
    QTemporaryFile *tempFile;
    int httpGetId;
    QHttp http;
    QUrl url;
    QTimer updateTimer;
};

#endif // NETWORKPICTURE_H
