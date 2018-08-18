#-------------------------------------------------
#
# Project created by QtCreator 2018-01-09T09:45:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ControlVelocidad
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

INCLUDEPATH += /us/include/
INCLUDEPATH += /usr/local/qwt-6.1.3/include


LIBS += -lpthread -lrt -lpigpio
LIBS += -L/usr/local/qwt-6.1.3/lib/ -lqwt
