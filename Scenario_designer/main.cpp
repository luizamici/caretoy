#include <QtGui>
#include "ct_scenarioconfig.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CTScenarioConfig *config = new CTScenarioConfig();
    config->setAttribute(Qt::WA_DeleteOnClose);
    config->show();

    return app.exec();
}
