QT += core gui xml sql opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = caretoyadmin
TEMPLATE = app
CONFIG -= app_bundle
CONFIG += DEBUG

SOURCES += main.cpp \
    mod_staff/ct_tableofpatients.cpp \
    mod_staff/ct_staffmodule.cpp \
    CareToy_Admin/ct_logindialog.cpp \
    CareToy_Admin/ct_loginadmin.cpp \
    CareToy_Admin/ct_admin.cpp \
    mod_staff/ct_staffmoduleadmin.cpp \
    Patients_Admin/ct_patientmodule.cpp \
    Patients_Admin/ct_patientmoduleadmin.cpp \
    Patients_Admin/ct_wizardlayout.cpp \
    Patients_Admin/ct_tableofscenarios.cpp \
    Patients_Admin/ct_tableofresults.cpp \
    Patients_Admin/ct_qsqltableofresults.cpp \
    Patients_Admin/ct_patientsform.cpp \
    mod_staff/ct_searchpatient.cpp \
    SecureSocketClient/ct_sslclientthread.cpp \
    DbTableXML/ct_queryparser.cpp \
    DbTableXML/ct_tabledata.cpp \
    DbTableXML/ct_tablefield.cpp \
    DbTableXML/ct_tablemodel.cpp \
    DbTableXML/ct_tablerecord.cpp \
    DbTableXML/ct_xmldataparser.cpp \
    DbTreeXml/ct_treemodel.cpp \
    mod_staff/ct_treeofworklogs.cpp \
    mod_staff/ct_worklogswidget.cpp \
    mod_staff/ct_calendar.cpp

HEADERS += \
    mod_staff/ct_tableofpatients.h \
    mod_staff/ct_staffmodule.h \
    CareToy_Admin/ct_logindialog.h \
    CareToy_Admin/ct_loginadmin.h \
    CareToy_Admin/ct_admin.h \
    CareToy_Admin/ct_defs.h \
    mod_staff/ct_staffmoduleadmin.h \
    Patients_Admin/ct_patientmodule.h \
    Patients_Admin/ct_patientmoduleadmin.h \
    Patients_Admin/ct_wizardlayout.h \
    Patients_Admin/ct_tableofscenarios.h \
    Patients_Admin/ct_tableofresults.h \
    Patients_Admin/ct_qsqltableofresults.h \
    Patients_Admin/ct_patientsform.h \
    mod_staff/ct_searchpatient.h \
    SecureSocketClient/ct_sslclientthread.h \
    DbTableXML/ct_queryparser.h \
    DbTableXML/ct_tabledata.h \
    DbTableXML/ct_tablefield.h \
    DbTableXML/ct_tablemodel.h \
    DbTableXML/ct_tablerecord.h \
    DbTableXML/ct_xmldataparser.h \
    DbTreeXml/ct_treemodel.h \
    mod_staff/ct_treeofworklogs.h \
    mod_staff/ct_worklogswidget.h \
    CareToy_Admin/ct_aux.h \
    mod_staff/ct_calendar.h


FORMS += \
    ct_logindialog.ui \
    ct_patientmodule.ui \
    ct_wizardlayout.ui \
    ct_searchpatient.ui \
    mod_staff/ct_worklogswidget.ui



unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../../usr/local/qwt-6.0.1/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../../../usr/local/qwt-6.0.1/include
DEPENDPATH += $$PWD/../../../../../../usr/local/qwt-6.0.1/include

RESOURCES += \
    extraSources.qrc
