#ifndef TEAROFFDATE_H
#define TEAROFFDATE_H

#include <QDate>
#include <QWidget>

class QLabel;

class TearOffDate : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QDate date
               READ getDate
               WRITE setDate)
public:
    explicit TearOffDate(QWidget *parent = 0);
    
    QDate getDate() const;

    void resizeEvent(QResizeEvent *);
    QSize minimumSizeHint() const;
signals:
    
public slots:
    void setDate(const QDate &date);
    void clear();

protected:
    QDate date;
    QLabel *month;
    QLabel *day;
    
    void calculateFontSizes();
};

#endif // TEAROFFDATE_H
