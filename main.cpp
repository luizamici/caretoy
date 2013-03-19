#include <QtGui>
#include "CareToy_Admin/ct_admin.h"
#include <QApplication>
#include "Patients_Admin/ct_wizard.h"


int main(int argc, char *argv[])
{


    QApplication::setDesktopSettingsAware(false);
    QApplication app(argc, argv);

    app.setStyleSheet("*[mandatoryField='true'] { border-width: 1px; border-color: red;  border-style: solid; }");
//    app.setStyle(new QWindowsStyle);

    CTScenariosAdmin admin;

    return app.exec();
}
