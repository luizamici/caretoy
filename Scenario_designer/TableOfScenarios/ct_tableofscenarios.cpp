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

    /*subwidget handling the list of scenarios*/
    tableOfScenarios = new CTSqlTable(this->parentWidget());
    QLayout *layout = this->layout();
    layout->addWidget(tableOfScenarios);

    statusBar = new QStatusBar();
    layout->addWidget(statusBar);

    /*alternative way for editing a scenario*/
    connect(tableOfScenarios,SIGNAL(doubleClicked(QModelIndex)),this,
            SLOT(on_qbt_edit_clicked()));
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
    if(tableOfScenarios->submitAll()){
        QPalette palette;
        palette.setColor( QPalette::WindowText, "green" );
        statusBar->setPalette( palette );
        statusBar->showMessage("Changes submitted to the DB successfully!",
                               5000);
    }
    else
    {
        QPalette palette;
        palette.setColor( QPalette::WindowText, "red" );
        statusBar->setPalette( palette );
        statusBar->showMessage("An error occurred while saving changes to the DB!"
                               +tableOfScenarios->tableModel->lastError().text(),
                               5000);
    }
}

void CTTableOfScenarios::save(QHash<QString, QString> scenario)
{
    if(tableOfScenarios->save(scenario))
    {
        QPalette palette;
        palette.setColor( QPalette::WindowText, "green" );
        statusBar->setPalette( palette );
        statusBar->showMessage("Scenario saved locally!",5000);
    }else{
        QPalette palette;
        palette.setColor( QPalette::WindowText, "red" );
        statusBar->setPalette( palette );
        statusBar->showMessage("An error occurred!",5000);
    }
}

CTTableOfScenarios::~CTTableOfScenarios()
{
    delete ui;
}
