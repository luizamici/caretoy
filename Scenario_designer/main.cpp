#include <QtGui>
#include "ct_scenarioconfig.h"

#include "TableOfScenarios/ct_tableofscenarios.h"
#include "TableOfScenarios/ct_logger.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("CaretoyAdmin");

    CTLogger logger;
    logger.initialize();

    CTTableOfScenarios *tableOfScenarios = new CTTableOfScenarios();
    tableOfScenarios->setAttribute(Qt::WA_DeleteOnClose);
    tableOfScenarios->show();

    CTScenarioConfig *config = new CTScenarioConfig();

    /*Temporary connections*/
    app.connect(tableOfScenarios,SIGNAL(newScenario()), config ,SLOT(show()));
    app.connect(tableOfScenarios,SIGNAL(newScenario()),config->scenarioCanvas,
                SLOT(resetScenario()));
    app.connect(tableOfScenarios,SIGNAL(editScenario(QHash<QString,QString>)),
                config->scenarioCanvas,SLOT(loadScenario(QHash<QString,QString>)));
    app.connect(tableOfScenarios,SIGNAL(editScenario(QHash<QString,QString>)),
                config,SLOT(show()));

    app.connect(config->scenarioCanvas,SIGNAL(save(QHash<QString,QString>)),
                tableOfScenarios,SLOT(save(QHash<QString,QString>)));
    app.connect(config->scenarioCanvas,SIGNAL(save(QHash<QString,QString>)),
                config,SLOT(close()));

    app.connect(tableOfScenarios,SIGNAL(destroyed()),config,SLOT(close()));

    Log4Qt::Logger::logger(QLatin1String("main"))->info("CaretoAdmin application started!");

    return app.exec();
}
