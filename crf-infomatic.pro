#-------------------------------------------------
#
# Project created by QtCreator 2015-02-06T20:48:10
#
#-------------------------------------------------

QT       += core gui network xml script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crf-infomatic
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    networkpicture.cpp \
    nextdeparturelabel.cpp \
    departureinfo.cpp \
    netstreampicture.cpp \
    digitalclock.cpp \
    nextcalendarevent.cpp

HEADERS  += dialog.h \
    networkpicture.h \
    nextdeparturelabel.h \
    departureinfo.h \
    netstreampicture.h \
    digitalclock.h \
    nextcalendarevent.h \
    calendarevent.h

FORMS    += dialog.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc
