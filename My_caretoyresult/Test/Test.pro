#-------------------------------------------------
#
# Project created by QtCreator 2012-10-12T18:49:24
#
#-------------------------------------------------

QT       += core gui

TARGET = Test
TEMPLATE = app


SOURCES += main.cpp\
        randomdata.cpp \
    plot.cpp

HEADERS  += randomdata.h \
    plot.h

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../../../../../usr/local/qwt-6.0.1/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../../../../../../usr/local/qwt-6.0.1/include
DEPENDPATH += $$PWD/../../../../../../../../../usr/local/qwt-6.0.1/include
