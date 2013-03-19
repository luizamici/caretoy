#include "ct_patientmodule.h"
#include "ui_ct_patientmodule.h"
#include <QSqlField>
#include "ct_wizard.h"
#include "ct_tableofscenarios.h"

//#include "mod_scenario/ct_scenarioconfig.h"

CTPatientModule::CTPatientModule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTPatientModule)
{
    ui->setupUi(this);
    this->setMinimumSize(1200,700);

    QFont serifFont("Times", 12, QFont::Bold);
    ui->qlbPatientData->setFont(serifFont);
    ui->qlbInternalData->setFont(serifFont);
    ui->qlbTrainingScenarios->setFont(serifFont);
    ui->qlbTrainingResults->setFont(serifFont);

    statusBar = new QStatusBar();
    statusBar->setMaximumHeight(20);
    statusBar->setFont(QFont("Helvetica [Cronyx]", 10));
    ui->horizontalLayout->addWidget(statusBar);

    warningMessage = new QMessageBox(QMessageBox::Warning,
                                     "Patient Module","Patient data incomplete!");

    warningMessage2 = new QMessageBox(QMessageBox::Warning,
                                      "Patient Module","Obligatory fields empty");
    confirmationMessage = new QMessageBox(QMessageBox::Warning,
                                          "Patient Module","Are you sure you"
                                          "wish to delete the selected scenario?");
    confirmationMessage->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    connect(ui->gobackButton,SIGNAL(clicked()),this, SIGNAL(beforeGoBack()));

}

void CTPatientModule::setPatientWizard(CTPatientsForm *patientForm)
{
    ui->qvbPatientData->addWidget(patientForm);
}

void CTPatientModule::setTableOfScenarios(CTTableOfScenarios *tableOfScenarios){
    ui->qvbScenarioData->addWidget(tableOfScenarios);
}

void CTPatientModule::setTableOfResults(CTTableOfResults *tableOfResults){
    ui->qvbResultData->addWidget(tableOfResults);
}



void CTPatientModule::showOkMessage(QString message){
    QPalette palette;
    palette.setColor( QPalette::WindowText, "green" );
    statusBar->setPalette( palette );
    statusBar->showMessage(message,5000);
}

void CTPatientModule::showErrorMessage(QString errorMessage){

    QPalette palette;
    palette.setColor( QPalette::WindowText, "red" );
    statusBar->setPalette( palette );
    statusBar->showMessage(errorMessage,5000);
}

void CTPatientModule::showWarningMessage(QString message)
{
    QPalette palette;
    palette.setColor( QPalette::WindowText, "black" );
    statusBar->setPalette( palette );
    statusBar->showMessage(message,5000);
}

void CTPatientModule::showWarningMessage1(){
    statusBar->showMessage("Please submit the new patient's data first by clicking Update!");
}


void CTPatientModule::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

CTPatientModule::~CTPatientModule()
{
    delete ui;
}

void CTPatientModule::on_addButton_clicked()
{
//    emit addButtonClicked();
    emit newScenario();
}

void CTPatientModule::on_editButton_clicked()
{

}

void CTPatientModule::on_copyButton_clicked()
{

}

void CTPatientModule::on_removeButton_clicked()
{

}
