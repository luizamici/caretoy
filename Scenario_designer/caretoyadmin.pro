QT += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = caretoyadmin
TEMPLATE = app
CONFIG -= app_bundle
CONFIG += DEBUG

include(/informatik/home/mici/git_caretoy/log4qt/src/log4qt/log4qt.pri)

SOURCES += main.cpp \
    ct_customwidgets.cpp \
    ct_conftoystick.cpp \
    ct_conftoyring.cpp \
    ct_conftoymickey.cpp \
    ct_conftoyflower.cpp \
    ct_conftoyu.cpp \
    ct_confarch.cpp \
    ct_confwallleft.cpp \
    ct_confwallright.cpp \
    ct_confwallscreen.cpp \
    ct_blockconfig.cpp \
    ct_simpleblock.cpp \
    ct_trashcan.cpp \
    ct_scenarioconfig.cpp \
    ct_scenariocanvas.cpp \
    ct_blocksource.cpp \
    TableOfScenarios/ct_dbconn.cpp \
    TableOfScenarios/ct_tableofscenarios.cpp \
    TableOfScenarios/ct_sqltable.cpp \
    TableOfScenarios/ct_sqlmodel.cpp \
    TableOfScenarios/ct_dialog.cpp \
    ct_xmlhandler.cpp \
    TableOfScenarios/ct_logger.cpp \
    TableOfScenariosFromXML/ct_xmltableofscenarios.cpp \
    TableOfScenariosFromXML/ct_xmlparser.cpp \
    TableOfScenariosFromXML/ct_viewofscenarios.cpp

HEADERS += \
    ct_customwidgets.h \
    ct_conftoystick.h \
    ct_conftoyring.h \
    ct_conftoymickey.h \
    ct_conftoyflower.h \
    ct_conftoyu.h \
    ct_confwallleft.h \
    ct_confwallright.h \
    ct_confwallscreen.h \
    ct_confarch.h \
    ct_blockconfig.h \
    ct_simpleblock.h \
    ct_trashcan.h \
    ct_scenarioconfig.h \
    ct_scenariocanvas.h \
    ct_blocksource.h \
    TableOfScenarios/ct_dbconn.h \
    TableOfScenarios/ct_tableofscenarios.h \
    TableOfScenarios/ct_sqltable.h \
    TableOfScenarios/ct_sqlmodel.h \
    TableOfScenarios/ct_dialog.h \
    ct_xmlhandler.h \
    TableOfScenarios/ct_logger.h \
    TableOfScenariosFromXML/ct_xmltableofscenarios.h \
    TableOfScenariosFromXML/ct_xmlparser.h \
    TableOfScenariosFromXML/ct_viewofscenarios.h

FORMS += \
    ct_conftoystick.ui \
    ct_conftoyring.ui \
    ct_conftoymickey.ui \
    ct_conftoyflower.ui \
    ct_conftoyu.ui \
    ct_confarch.ui \
    ct_confwallleft.ui \
    ct_confwallright.ui \
    ct_confwallscreen.ui \
    ct_blockconfig.ui \
    ct_simpleblock.ui \
    TableOfScenarios/ct_tableofscenarios.ui \
    TableOfScenarios/ct_dialog.ui

RESOURCES += \
    ct_resources.qrc
