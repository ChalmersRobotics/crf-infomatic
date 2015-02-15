#include "quotelabel.h"

#include <QRegExp>
#include <QTextStream>


QuoteLabel::QuoteLabel(QWidget *parent) :
    QLabel(parent),
    quoteFile(0),
    httpGetId(0)
{
    connect(&http, SIGNAL(requestFinished(int,bool)),
            this, SLOT(httpRequestFinished(int,bool)));
    connect(&syncTimer, SIGNAL(timeout()),
            this, SLOT(updateQuoteFile()));
    connect(&changeTimer, SIGNAL(timeout()),
            this, SLOT(showRandomQuote()));

    syncTimer.start(60*60*24*1000); // 1/day
    changeTimer.start(10000); // Every 10 seconds
}

QUrl QuoteLabel::getUrl() const
{
    return url;
}

int QuoteLabel::getQuoteSyncInterval() const
{
    return syncTimer.interval();
}

int QuoteLabel::getQuoteChangeInterval() const
{
    return changeTimer.interval();
}

void QuoteLabel::setUrl(const QUrl &newUrl)
{
    url = newUrl;
    updateQuoteFile();
}

void QuoteLabel::setQuotesSyncInterval(int seconds)
{
    syncTimer.stop();
    syncTimer.setInterval(seconds * 1000);
    if (seconds != 0) {
        syncTimer.start();
    }
}

void QuoteLabel::setQuotesChangeInterval(int seconds)
{
    changeTimer.stop();
    changeTimer.setInterval(seconds * 1000);
    if (seconds != 0) {
        changeTimer.start();
    }
}

void QuoteLabel::showRandomQuote()
{
    if (quotes.empty()) {
        setText("No quotes loaded");
    } else {
        setText(quotes.at(qrand() % quotes.size()));
    }
}

void QuoteLabel::updateQuoteFile()
{
    if (!url.isValid()) {
        return;
    }

    delete quoteFile;
    quoteFile = new QTemporaryFile();
    if (!quoteFile->open()) {
        delete quoteFile;
        quoteFile = 0;
        return;
    }

    http.clearPendingRequests();
    http.setHost(url.host(), QHttp::ConnectionModeHttps);
    httpGetId = http.get(url.encodedPath(), quoteFile);
}

void QuoteLabel::httpRequestFinished(int id, bool error)
{
    if (error) {
        setText(http.errorString());
    } else if (id == httpGetId) {
        quoteFile->seek(0);
        QTextStream fileStream(quoteFile);
        readQuotes(fileStream);
        showRandomQuote();
    }
}

void QuoteLabel::readQuotes(QTextStream &in)
{
    if (!quoteFile) {
        return;
    }

    quotes.clear();
    QString line = in.readLine();
    while (!in.atEnd()) {
        if ( ! (line.isEmpty() ||
                line.contains("CRF citat:") ||
                // Filter out all lines starting with a year
                line.contains(QRegExp("^[0-9]{4,4}")) ||
                line.contains("Från IRC"))
            ) {
            quotes << line;
        }
        line = in.readLine();
    }
}
