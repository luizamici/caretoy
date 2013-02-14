#include "ct_scenarioconfig.h"

CTScenarioConfig::CTScenarioConfig(QWidget *parent) : QWidget(parent)
{

    Log4Qt::Logger::logger("CTScenarioConfig")->info(
                "Layout of the scenario configuration widget being created ...");
    // Initialize/configure visual appearance
    this->setWindowTitle(tr("Scenario configuration[*]"));
    this->setFixedSize(1100, 700);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(10);
    layout->setSpacing(10);
    this->setLayout(layout);

    QHBoxLayout *scenarioLayout = new QHBoxLayout();
    scenarioLayout->setMargin(0);
    scenarioLayout->setSpacing(10);
    layout->addLayout(scenarioLayout);

    // Add scroll area and widget for scenario block arrangement
    QScrollArea *qsaScenario = new QScrollArea();
    qsaScenario->setWidgetResizable(true);
    qsaScenario->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    qsaScenario->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    qsaScenario->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scenarioLayout->addWidget(qsaScenario);
    scenarioCanvas = new CTScenarioCanvas();
    qsaScenario->setWidget(scenarioCanvas);
    scenarioCanvas->initialize();

    QVBoxLayout *blockLayout = new QVBoxLayout();
    blockLayout->setMargin(0);
    blockLayout->setSpacing(10);
    scenarioLayout->addLayout(blockLayout);

    // Add scroll area and widget containing the available blocks
    QScrollArea *qsaBlocks = new QScrollArea();
    qsaBlocks->setWidgetResizable(true);
    qsaBlocks->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    qsaBlocks->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    qsaBlocks->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    blockLayout->addWidget(qsaBlocks);
    CTBlockSource *blockSource = new CTBlockSource();
    qsaBlocks->setWidget(blockSource);
    blockSource->initialize();
    CTTrashCan *trashCan = new CTTrashCan();
    blockLayout->addWidget(trashCan);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(10);
    layout->addLayout(buttonLayout);

    QPushButton *qpbReset = new QPushButton("Reset");
    buttonLayout->addWidget(qpbReset);
    buttonLayout->addStretch();
    QPushButton *qpbSaveToDB = new QPushButton("Save");
    buttonLayout->addWidget(qpbSaveToDB);
    qpbCancel = new QPushButton("Cancel");
    buttonLayout->addWidget(qpbCancel);

    // Establish connections
    connect(qpbReset, SIGNAL(clicked()), scenarioCanvas, SLOT(resetScenario()));
    connect(qpbSaveToDB,SIGNAL(clicked()),this, SLOT(saveScenarioToDB()));
    connect(qpbCancel, SIGNAL(clicked()), this, SLOT(close()));

    connect(this, SIGNAL(destroyed()),scenarioCanvas,SIGNAL(destroyed()));

    // Center the widget on the screen
    QRect desktop = QApplication::desktop()->availableGeometry();
    this->move(desktop.width()/2 - this->width()/2, desktop.height()/2 - this->height()/2);

    Log4Qt::Logger::logger("CTScenarioConfig")->info(
                "Layout of the scenario configuration widget ready.");
}

void CTScenarioConfig::saveScenarioToDB()
{
    dialog = new CTDialog();
    connect(qpbCancel, SIGNAL(clicked()),dialog, SLOT(close()));

    connect(dialog,SIGNAL(accepted(QString,QString,QString)),scenarioCanvas,
            SLOT(saveScenario(QString,QString,QString)));

    if(!scenarioCanvas->isNewScenario())
    {
        dialog->setData(scenarioCanvas->getDescription(),
                        scenarioCanvas->getExecutionDay(),
                        scenarioCanvas->getExecutionOrder());
    }
    Qt::WindowFlags flags = dialog->windowFlags();
    flags ^= Qt::WindowStaysOnTopHint;
    dialog->setWindowFlags( flags );
    dialog->show();
    dialog->activateWindow();
}

