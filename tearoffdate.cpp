#include "tearoffdate.h"

#include <QFont>
#include <QLabel>
#include <QRect>
#include <QSizePolicy>
#include <QVBoxLayout>


TearOffDate::TearOffDate(QWidget *parent) :
    QWidget(parent),
    month(new QLabel(this)),
    day(new QLabel(this))
{
    QPalette palette;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignHCenter);
    layout->setSpacing(0);
    layout->setMargin(0);

    month->setAutoFillBackground(true);
    palette = month->palette();
    palette.setColor(QPalette::Window, Qt::red);
    month->setPalette(palette);
    month->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    month->setSizePolicy(QSizePolicy::MinimumExpanding,
                         QSizePolicy::MinimumExpanding);
    layout->addWidget(month);
    layout->setStretchFactor(month, 1);

    day->setAutoFillBackground(true);
    palette = day->palette();
    palette.setColor(QPalette::Window, Qt::darkRed);
    day->setPalette(palette);
    day->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    day->setSizePolicy(QSizePolicy::MinimumExpanding,
                       QSizePolicy::MinimumExpanding);
    layout->addWidget(day);
    layout->setStretchFactor(day, 2);

    this->setLayout(layout);
}

QDate TearOffDate::getDate() const
{
    return date;
}

void TearOffDate::resizeEvent(QResizeEvent *)
{
    calculateFontSizes();
}

QSize TearOffDate::minimumSizeHint() const
{
    return QSize(10, 10);
}

void TearOffDate::setDate(const QDate &date)
{
    if (date.isValid()) {
        this->date = date;
        month->setText(QDate::shortMonthName(date.month(), QDate::StandaloneFormat));
        day->setNum(date.day());
        calculateFontSizes();
    }
}

void TearOffDate::clear()
{
    date = QDate();
    month->clear();
    day->clear();
}

void TearOffDate::calculateFontSizes()
{
    qreal f1, f2;
    QRectF br;

    if (!month->text().isEmpty()) {
        br = month->fontMetrics().boundingRect(month->text());
        f1 = month->width() / br.width();
        f2 = month->height() / br.height();
        if ((f1 < 1) || (f2 < 1) || (f1 > 1.25) || (f2 > 1.25)) {
            QFont font = month->font();
            if (f1 < f2) {
                font.setPointSizeF(font.pointSizeF() * f1);
            } else {
                font.setPointSizeF(font.pointSizeF() * f2);
            }
            month->setFont(font);
        }
    }

    if (!day->text().isEmpty()) {
        br = day->fontMetrics().boundingRect(day->text());
        f1 = day->width() / br.width();
        f2 = day->height() / br.height();
        if ((f1 < 1) || (f2 < 1) || (f1 > 1.25) || (f2 > 1.25)) {
            QFont font = day->font();
            if (f1 < f2) {
                font.setPointSizeF(font.pointSizeF() * f1);
            } else {
                font.setPointSizeF(font.pointSizeF() * f2);
            }
            day->setFont(font);
        }
    }
}
