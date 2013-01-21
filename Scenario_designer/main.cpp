#include <QtGui>
#include "ct_scenarioconfig.h"
#include "ct_logger.h"
#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CTLogger logger;
    logger.initialize(app.applicationDirPath());

    CTScenarioConfig *config = new CTScenarioConfig();
    config->setAttribute(Qt::WA_DeleteOnClose);
    config->show();


    Log4Qt::Logger::logger(QLatin1String("main"))->info(
                "CareToy scenario designer is being executed!");

    int out = app.exec();
    if(out == 0)
    {
        Log4Qt::Logger::logger(QLatin1String("main"))->info(
                    "CareToy scenario designer is about to quit!");

    }
    return out;
}
