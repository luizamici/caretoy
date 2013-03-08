#include <QtGui>
#include "ct_logger.h"
#include "CareToy_Admin/ct_admin.h"
//#include "TableOfScenariosFromXML/ct_scenariosadmin.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("CaretoyAdmin");

    CTLogger logger;
    logger.initialize(app.applicationDirPath());

    CTAdmin admin;
//    CTScenariosAdmin admin;


    Log4Qt::Logger::logger(QLatin1String("main"))->info(
                "CaretoAdmin application started!");

    return app.exec();
}
