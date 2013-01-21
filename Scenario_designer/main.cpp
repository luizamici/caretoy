#include <QtGui>
#include "ct_scenarioconfig.h"
#include "ct_logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CTLogger logger;
    logger.initialize(app.applicationDirPath());

    CTScenarioConfig *config = new CTScenarioConfig();
    config->setAttribute(Qt::WA_DeleteOnClose);
    config->show();


    Log4Qt::Logger::logger(QLatin1String("main"))->info(
                "CareToy designer is being executed!");
    return app.exec();
}
