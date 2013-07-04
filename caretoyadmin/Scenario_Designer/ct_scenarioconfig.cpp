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
    QPushButton *qpbLoad = new QPushButton("Load");
    buttonLayout->addWidget(qpbLoad);

    // Establish connections
    connect(qpbReset, SIGNAL(clicked()), scenarioCanvas, SLOT(
                resetScenario()));
    connect(qpbSaveToDB,SIGNAL(clicked()),this, SLOT(saveScenarioToDB()));
    connect(qpbCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(qpbCancel,SIGNAL(clicked()),scenarioCanvas,SLOT(resetScenario()));

    connect(qpbLoad, SIGNAL(clicked()),scenarioCanvas, SLOT(
                loadScenarioFromFile()));

    connect(scenarioCanvas,SIGNAL(titleChanged(QString)),this, SLOT(
                setWindowTitle(QString)));
    connect(this, SIGNAL(destroyed()),scenarioCanvas,SIGNAL(destroyed()));

    // Center the widget on the screen
    QRect desktop = QApplication::desktop()->availableGeometry();
    this->move(desktop.width()/2 - this->width()/2, desktop.height()/2 -
               this->height()/2);

    Log4Qt::Logger::logger("CTScenarioConfig")->info(
                "Layout of the scenario configuration widget ready.");
}

void CTScenarioConfig::closeEvent(QCloseEvent *ev)
{
    scenarioCanvas->resetScenario();
    return;
}

void CTScenarioConfig::openScenario()
{
    scenarioCanvas->loadScenario();
}

void CTScenarioConfig::saveScenarioToDB()
{
    wizard = new CTWizard();

    connect(qpbCancel, SIGNAL(clicked()),wizard, SLOT(close()));
    connect(wizard, SIGNAL(save()), scenarioCanvas, SLOT(
                saveScenario()));
//    if(!scenarioCanvas->isNewScenario())
//    {
//        wizard->setInputData(CTScenarioData::instance().data()->description,
//                             CTScenarioData::instance().data()->execution_day,
//                             CTScenarioData::instance().data()->execution_order,
//                             CTScenarioData::instance().data()->image_description,
//                             CTScenarioData::instance().data()->position_image);
//        wizard->setOutcomeMeasures(CTScenarioData::instance().data()->outcome_measures);
//    }
    wizard->initialize(scenarioCanvas->isNewScenario());

    Qt::WindowFlags flags = wizard->windowFlags();
    flags ^= Qt::WindowStaysOnTopHint;
    wizard->setWindowFlags( flags );
    wizard->show();
    wizard->raise();
    wizard->activateWindow();
}

