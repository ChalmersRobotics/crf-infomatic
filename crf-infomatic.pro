#-------------------------------------------------
#
# Project created by QtCreator 2015-02-06T20:48:10
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crf-infomatic
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    networkpicture.cpp

HEADERS  += dialog.h \
    networkpicture.h

FORMS    += dialog.ui
