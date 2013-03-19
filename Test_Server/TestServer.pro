#-------------------------------------------------
#
# Project created by QtCreator 2013-02-04T16:12:39
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = TestServer
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp \
    queryparser.cpp \
    dbconn.cpp \
    datatoxml.cpp

HEADERS  += server.h \
    queryparser.h \
    dbconn.h \
    datatoxml.h \
    ct_defs.h

RESOURCES += \
    source.qrc
