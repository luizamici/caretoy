#ifndef CT_SCENARIOCONFIG_H
#define CT_SCENARIOCONFIG_H

#include <QtGui>
#include "ct_scenariocanvas.h"
#include "ct_blocksource.h"
#include "ct_trashcan.h"
#include "TableOfScenarios/ct_logger.h"

class CTScenarioConfig : public QWidget
{
    Q_OBJECT

public:
    explicit CTScenarioConfig(QWidget *parent = 0);

    CTScenarioCanvas *scenarioCanvas;
private:
    CTDialog *dialog;
    QPushButton *qpbCancel;

private slots:
    void saveScenarioToDB();

public slots:
    void openScenario(QHash<QString, QString> scenario);

};

#endif // CT_SCENARIOCONFIG_H
