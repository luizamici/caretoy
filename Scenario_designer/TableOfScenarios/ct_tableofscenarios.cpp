#include "ct_tableofscenarios.h"
#include "ui_ct_tableofscenarios.h"

CTTableOfScenarios::CTTableOfScenarios(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTTableOfScenarios)
{
    ui->setupUi(this);
    this->setWindowTitle("Scenarios");

    QRect desktop = QApplication::desktop()->availableGeometry();
    this->move(desktop.width()/2 - this->width()/2,
               desktop.height()/2 - this->height()/2);

    tableOfScenarios = new CTSqlTable(this->parentWidget());
    QLayout *layout = this->layout();
    layout->addWidget(tableOfScenarios);

    statusBar = new QStatusBar();
    layout->addWidget(statusBar);
}

CTTableOfScenarios::~CTTableOfScenarios()
{
    delete ui;
}

void CTTableOfScenarios::on_qbt_add_clicked()
{
    emit newScenario();
}

void CTTableOfScenarios::on_qbt_edit_clicked()
{
    QHash<QString,QString> scenario = tableOfScenarios->getSelected();
    emit editScenario(scenario);
}

void CTTableOfScenarios::on_qbt_copy_clicked()
{
    QSqlRecord scenarioRecord = tableOfScenarios->getSelectedRecord();
    tableOfScenarios->copyIntoTable(scenarioRecord);
}

void CTTableOfScenarios::on_qbt_delete_clicked()
{
    QHash<QString,QString> scenario = tableOfScenarios->getSelected();
    tableOfScenarios->deleteFromTable(scenario["id"]);
}

void CTTableOfScenarios::on_qbt_submit_clicked()
{
    tableOfScenarios->submitAll();
}
