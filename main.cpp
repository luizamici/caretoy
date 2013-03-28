#include <QtGui>
#include "CareToy_Admin/ct_admin.h"
#include <QApplication>
#include "Patients_Admin/ct_wizard.h"

int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(false);
    QApplication app(argc, argv);
    QString s("Ciao!");
    app.setProperty("global_variable", s);

    app.setStyleSheet("*[mandatoryField='true'] { border-width: 1px; border-color: red;  border-style: solid; }");
//    app.setStyle(new QWindowsStyle);

    CTAdmin admin;

    return app.exec();
}
