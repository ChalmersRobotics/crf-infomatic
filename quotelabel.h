#ifndef QUOTELABEL_H
#define QUOTELABEL_H

#include <QLabel>
#include <QHttp>
#include <QString>
#include <QStringList>
#include <QTemporaryFile>
#include <QTimer>
#include <QUrl>

class QTextStream;

class QuoteLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ getUrl WRITE setUrl)
    Q_PROPERTY(int quoteSyncInterval
               READ getQuoteSyncInterval
               WRITE setQuotesSyncInterval
               STORED false)
    Q_PROPERTY(int quoteChangeInterval
               READ getQuoteChangeInterval
               WRITE setQuotesChangeInterval
               STORED false)

public:
    explicit QuoteLabel(QWidget *parent = 0);
        
    QUrl getUrl() const;
    int getQuoteSyncInterval() const;
    int getQuoteChangeInterval() const;
    
public slots:
    void setUrl(const QUrl &newUrl);
    void setQuotesSyncInterval(int seconds);
    void setQuotesChangeInterval(int seconds);
    void showRandomQuote();

protected slots:
    void updateQuoteFile();
    void httpRequestFinished(int, bool);

protected:
    QTemporaryFile *quoteFile;
    int httpGetId;
    QHttp http;
    QUrl url;
    QTimer syncTimer;
    QTimer changeTimer;
    QStringList quotes;

    void readQuotes(QTextStream &in);
};

#endif // QUOTELABEL_H
