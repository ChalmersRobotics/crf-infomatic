#ifndef NETWORKPICTURE_H
#define NETWORKPICTURE_H

#include <QHttp>
#include <QUrl>
#include <QLabel>

class QTemporaryFile;

class NetworkPicture : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ getUrl WRITE setUrl)

public:
    explicit NetworkPicture(QWidget *parent = 0);
    
    QUrl getUrl() const;
    void setUrl(const QUrl &newUrl);

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
};

#endif // NETWORKPICTURE_H
