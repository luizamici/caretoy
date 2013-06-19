#-------------------------------------------------
#
# Project created by QtCreator 2012-10-04T14:41:11
#
#-------------------------------------------------

QT       += core gui opengl phonon

TARGET = caretoyresult
TEMPLATE = app


SOURCES += main.cpp\
        ct_resultview.cpp \
    ct_plot.cpp \
    ct_videoview.cpp \
    ct_dataview.cpp \
    ct_plotmagnifier.cpp \
    ct_plotzoomer.cpp \
    ct_mediaobject.cpp \
    ct_graphviewer.cpp

HEADERS  += ct_resultview.h \
    ct_plot.h \
    ct_videoview.h \
    ct_dataview.h \
    ct_plotmagnifier.h \
    ct_plotzoomer.h \
    ct_mediaobject.h \
    ct_graphviewer.h

FORMS    += \
     ct_resultview.ui \
    ct_videoview.ui \
    ct_graphviewer.ui

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../../usr/local/qwt-6.0.2/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../../../usr/local/qwt-6.0.2/include
DEPENDPATH += $$PWD/../../../../../../usr/local/qwt-6.0.2/include

RESOURCES += \
    input_source.qrc
