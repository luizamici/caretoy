#include <QtGui>
#include "ct_logger.h"
#include "TableOfScenariosFromXML/ct_admin.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("CaretoyAdmin");

    CTLogger logger;
    logger.initialize(app.applicationDirPath());

    CTAdmin admin;


    Log4Qt::Logger::logger(QLatin1String("main"))->info(
                "CaretoAdmin application started!");

    return app.exec();
}
