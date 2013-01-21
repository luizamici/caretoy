#ifndef CT_SCENARIOCONFIG_H
#define CT_SCENARIOCONFIG_H

#include <QtGui>
#include "ct_scenariocanvas.h"
#include "ct_blocksource.h"
#include "ct_trashcan.h"

#include "ct_logger.h"

class CTScenarioConfig : public QWidget
{
    Q_OBJECT

public:
    explicit CTScenarioConfig(QWidget *parent = 0);

    CTScenarioCanvas *scenarioCanvas;
};

#endif // CT_SCENARIOCONFIG_H
