QT += core gui xml sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = caretoyadmin
TEMPLATE = app
CONFIG -= app_bundle
CONFIG += DEBUG

include(./include/log4qt/src/log4qt/log4qt.pri)

SOURCES += main.cpp \
    Scenario_Designer/ct_customwidgets.cpp \
    Scenario_Designer/ct_conftoymickey.cpp \
    Scenario_Designer/ct_conftoyu.cpp \
    Scenario_Designer/ct_confarch.cpp \
    Scenario_Designer/ct_confwallleft.cpp \
    Scenario_Designer/ct_confwallright.cpp \
    Scenario_Designer/ct_confwallscreen.cpp \
    Scenario_Designer/ct_blockconfig.cpp \
    Scenario_Designer/ct_simpleblock.cpp \
    Scenario_Designer/ct_trashcan.cpp \
    Scenario_Designer/ct_scenarioconfig.cpp \
    Scenario_Designer/ct_scenariocanvas.cpp \
    Scenario_Designer/ct_blocksource.cpp \
    Scenario_Designer/ct_xmlhandler.cpp \
    ct_logger.cpp \
    TableOfScenariosFromXML/ct_viewofscenarios.cpp \
    TableOfScenariosFromXML/ct_queryparser.cpp \
    TableOfScenariosFromXML/ct_xmldataparser.cpp \
    TableOfScenariosFromXML/ct_tablemodel.cpp \
    TableOfScenariosFromXML/ct_tabledata.cpp \
    TableOfScenariosFromXML/ct_tablefield.cpp \
    TableOfScenariosFromXML/ct_tablerecord.cpp \
    SecureSocketClient/ct_sslclientthread.cpp \
    CareToy_Admin/ct_logindialog.cpp \
    TableOfScenariosFromXML/ct_scenariosadmin.cpp \
    CareToy_Admin/ct_admin.cpp \
    Scenario_Designer/ct_wizard.cpp \
    Scenario_Designer/ct_wizardpage.cpp \
    Scenario_Designer/ct_conftoylring.cpp \
    Scenario_Designer/ct_conftoysring.cpp \
    ct_proxymodel.cpp


HEADERS += \
    CareToy_Admin/ct_defs.h \
    Scenario_Designer/ct_customwidgets.h \
    Scenario_Designer/ct_conftoymickey.h \
    Scenario_Designer/ct_conftoyu.h \
    Scenario_Designer/ct_confwallleft.h \
    Scenario_Designer/ct_confwallright.h \
    Scenario_Designer/ct_confwallscreen.h \
    Scenario_Designer/ct_confarch.h \
    Scenario_Designer/ct_blockconfig.h \
    Scenario_Designer/ct_simpleblock.h \
    Scenario_Designer/ct_trashcan.h \
    Scenario_Designer/ct_scenarioconfig.h \
    Scenario_Designer/ct_scenariocanvas.h \
    Scenario_Designer/ct_blocksource.h \
    Scenario_Designer/ct_xmlhandler.h \
    ct_logger.h \
    TableOfScenariosFromXML/ct_viewofscenarios.h \
    TableOfScenariosFromXML/ct_queryparser.h \
    TableOfScenariosFromXML/ct_xmldataparser.h \
    TableOfScenariosFromXML/ct_tablemodel.h \
    TableOfScenariosFromXML/ct_tabledata.h \
    TableOfScenariosFromXML/ct_tablefield.h \
    TableOfScenariosFromXML/ct_tablerecord.h \
    SecureSocketClient/ct_sslclientthread.h \
    CareToy_Admin/ct_logindialog.h \
    TableOfScenariosFromXML/ct_scenariosadmin.h \
    CareToy_Admin/ct_admin.h \
    Scenario_Designer/ct_wizard.h \
    Scenario_Designer/ct_wizardpage.h \
    Scenario_Designer/ct_conftoylring.h \
    Scenario_Designer/ct_conftoysring.h \
    ct_scenariodata.h \
    ct_proxymodel.h

FORMS += \
    Scenario_Designer/ct_conftoymickey.ui \
    Scenario_Designer/ct_conftoyu.ui \
    Scenario_Designer/ct_confarch.ui \
    Scenario_Designer/ct_confwallleft.ui \
    Scenario_Designer/ct_confwallright.ui \
    Scenario_Designer/ct_confwallscreen.ui \
    Scenario_Designer/ct_blockconfig.ui \
    Scenario_Designer/ct_simpleblock.ui \
    CareToy_Admin/ct_logindialog.ui \
    Scenario_Designer/ct_wizard.ui \
    Scenario_Designer/ct_conftoylring.ui \
    Scenario_Designer/ct_conftoysring.ui

RESOURCES += \
    ct_resources.qrc
