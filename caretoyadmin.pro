QT += core gui xml sql opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = caretoyadmin
TEMPLATE = app
CONFIG -= app_bundle
CONFIG += DEBUG

SOURCES += main.cpp \
    mod_staff/ct_tableofpatients.cpp \
    mod_staff/ct_staffmodule.cpp \
    CareToy_Admin/ct_logindialog.cpp \
    CareToy_Admin/ct_dbconn.cpp \
    CareToy_Admin/ct_loginadmin.cpp \
    CareToy_Admin/ct_admin.cpp \
    CareToy_Admin/ct_dbconnadmin.cpp \
    mod_staff/ct_staffmoduleadmin.cpp \
    mod_staff/ct_worklogs.cpp \
    mod_staff/ct_qsqltableofpatients.cpp \
    Patients_Admin/ct_patientmodule.cpp \
    Patients_Admin/ct_patientmoduleadmin.cpp \
    Patients_Admin/ct_wizardlayout.cpp \
    Patients_Admin/ct_tableofscenarios.cpp \
    Patients_Admin/ct_qsqltableofscenarios.cpp \
    Patients_Admin/ct_tableofresults.cpp \
    Patients_Admin/ct_qsqltableofresults.cpp \
    Patients_Admin/ct_patientsform.cpp \
    mod_staff/ct_searchpatient.cpp

HEADERS += \
    mod_staff/ct_dbconn.h \
    mod_staff/ct_tableofpatients.h \
    mod_staff/ct_staffmodule.h \
    CareToy_Admin/ct_logindialog.h \
    CareToy_Admin/ct_dbconn.h \
    CareToy_Admin/ct_loginadmin.h \
    CareToy_Admin/ct_admin.h \
    CareToy_Admin/ct_dbconnadmin.h \
    mod_staff/ct_staffmoduleadmin.h \
    mod_staff/ct_worklogs.h \
    mod_staff/ct_qsqltableofpatients.h \
    Patients_Admin/ct_patientmodule.h \
    Patients_Admin/ct_patientmoduleadmin.h \
    Patients_Admin/ct_wizardlayout.h \
    Patients_Admin/ct_tableofscenarios.h \
    Patients_Admin/ct_qsqltableofscenarios.h \
    Patients_Admin/ct_tableofresults.h \
    Patients_Admin/ct_qsqltableofresults.h \
    Patients_Admin/ct_patientsform.h \
    mod_staff/ct_searchpatient.h


FORMS += \
    ct_logindialog.ui \
    ct_dialogs.ui \
    ct_patientmodule.ui \
    ct_wizardlayout.ui \
    ct_searchpatient.ui



unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../../usr/local/qwt-6.0.1/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../../../usr/local/qwt-6.0.1/include
DEPENDPATH += $$PWD/../../../../../../usr/local/qwt-6.0.1/include

RESOURCES += \
    extraSources.qrc
