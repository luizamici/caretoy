#ifndef CT_SCENARIOCONFIG_H
#define CT_SCENARIOCONFIG_H

#include <QtGui>
#include "ct_scenariocanvas.h"
#include "ct_blocksource.h"
#include "ct_trashcan.h"
#include "ct_logger.h"
#include "ct_wizard.h"

class CTScenarioConfig : public QWidget
{
    Q_OBJECT

public:
    explicit CTScenarioConfig(QWidget *parent = 0);

    CTScenarioCanvas *scenarioCanvas;
private:
    CTWizard *wizard;
    QPushButton *qpbCancel;
    QLCDNumber *runtime;

    void closeEvent(QCloseEvent * ev);

private slots:
    void saveScenarioToDB();

public slots:
    void openScenario();
    void setRuntime(double total_runtime);

};

#endif // CT_SCENARIOCONFIG_H
