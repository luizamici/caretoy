#include "ct_tableofscenarios.h"
#include "ui_ct_tableofscenarios.h"

CTTableOfScenarios::CTTableOfScenarios(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTTableOfScenarios)
{
    ui->setupUi(this);
    this->setWindowTitle("Scenarios");

    tableOfScenarios = new CTSqlTable(this->parentWidget());
    QLayout *layout = this->layout();
    layout->addWidget(tableOfScenarios);
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

}

void CTTableOfScenarios::on_qbt_delete_clicked()
{

}

void CTTableOfScenarios::on_qbt_submit_clicked()
{
    tableOfScenarios->submitAll();
}
