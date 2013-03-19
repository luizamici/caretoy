#include "ct_patientmoduleadmin.h"
#include "ct_patientmodule.h"
#include "ct_patientsform.h"

CTPatientModuleAdmin::CTPatientModuleAdmin(QWidget *parent) :
    QWidget(parent)
{
    patientModule = new CTPatientModule();

    form = new CTPatientsForm();
    patientModule->setPatientWizard(form);

    tableOfScenarios = new CTTableOfScenarios();
    patientModule->setTableOfScenarios(tableOfScenarios);

    tableOfResults = new CTTableOfResults();
    patientModule->setTableOfResults(tableOfResults);

    confirmationMessage = new QMessageBox(QMessageBox::Warning,
                                          tr("Patient Module"),
                                          tr("The patient has been modified.\n"
                                             "Do you wish to discard your changes?"));
    confirmationMessage->setStandardButtons(QMessageBox::Discard
                                            | QMessageBox::Cancel);


    connect(form,SIGNAL(nothingToSave(QString)),patientModule,
            SLOT(showWarningMessage(QString)));
    connect(form,SIGNAL(insert(QHash<QString,QString>)),this,
            SIGNAL(newPatientAdded(QHash<QString,QString>)));
    connect(form,SIGNAL(update(QHash<QString,QString>)),this,
            SIGNAL(selectedPatientChanged(QHash<QString,QString>)));

    connect(tableOfScenarios,SIGNAL(checkForResults(QString)),
            this, SLOT(searchResults(QString)));

    connect(patientModule,SIGNAL(beforeGoBack()),this, SLOT(goBack()));
    connect(patientModule,SIGNAL(addButtonClicked()),this, SLOT(checkAddButtonClicked()));
}

void CTPatientModuleAdmin::editSelectedPatient(
        QHash<QString,QString> patient,
        CTQSqlTableOfScenarios *sqlTableOfpatients,
        CTQSqlTableOfResults *sqlTableOfResults){

    newPatient = false;
    /*
     *The selected-for-editting patient's data is stored into a local variable
     *in order to check for changed data through the patient module interface
     */
    localPatientData.clear();
    localPatientData = patient;
    form->initializeFormWithPatient(localPatientData);
    tableOfScenarios->setQSqlModel(sqlTableOfpatients);
    tableOfResults->setQSqlModel(sqlTableOfResults);
    patientModule->show();
}


void CTPatientModuleAdmin::openNewPatientDialog(QStringList idList){

    newPatient = true;
    localPatientData.clear();
    form->initializeNewPatient();
    form->setIdList(idList);
    tableOfScenarios->setQSqlModel(new CTQSqlTableOfScenarios());
    patientModule->show();
}

void CTPatientModuleAdmin::searchResults(QString id_scenario){
    tableOfResults->filterByScenario(id_scenario);
}


void CTPatientModuleAdmin::updateRow(QString message, QString row){
    patientModule->showOkMessage(message);
    form->setRow(row);
}


void CTPatientModuleAdmin::goBack(){

    if(form->unsavedChanges()){
        showConfirmationDialog();
    }else{
        patientModule->close();
    }
}

void CTPatientModuleAdmin::checkAddButtonClicked()
{
    /*
     *Checks if the update button is enabled knowing that the button
     *takes care of patient data consistency
     */
    if(!form->updateButtonEnabled())
    {
        patientModule->warningMessage->show();
    }
    else
    {
        //TODO: open a new scenario designer dialog
    }
}

void CTPatientModuleAdmin::showConfirmationDialog(){
    int ret = confirmationMessage->exec();
    switch(ret){
        case QMessageBox::Discard :
            patientModule->close();
        break;
    default:
        break;
    }
}


bool CTPatientModuleAdmin::isEmpty(QHash<QString, QString> p){
    bool empty = true;
    p["row"] = "";
    foreach(QString key, p.keys()){
        if(!p[key].trimmed().isEmpty()){
            empty = false;
        }
    }
    return empty;

}


/*
 *In case the user has inserted changes to the patient's data in the user interface
 */
void CTPatientModuleAdmin::updateLocalPatientData(QHash<QString,QString> patientDataFromUI){
    foreach(QString key, patientDataFromUI.keys()){
       localPatientData[key] = patientDataFromUI[key];
    }
}


void CTPatientModuleAdmin::showErrorMessage(QString errorMessage){
}



