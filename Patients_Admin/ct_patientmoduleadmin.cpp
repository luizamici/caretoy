#include "ct_patientmoduleadmin.h"
#include "ct_patientmodule.h"
#include "ct_patientsform.h"

#include "CareToy_Admin/ct_defs.h"
#include "DbTableXML/ct_xmldataparser.h"

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

void CTPatientModuleAdmin::initEdit(QHash<QString, QString> patient)
{
    form->newPatient = false;
    /*
     *The selected-for-editting patient's data is stored into a local variable
     *in order to check for changed data through the patient module interface
     */
    localPatientData.clear();
    localPatientData = patient;
    form->initializeFormWithPatient(localPatientData);
    //tableOfResults.init()
    patientModule->show();
    requestTableOfScenarios();
}


void CTPatientModuleAdmin::openNewPatientDialog(QStringList idList){

    form->newPatient = true;
    localPatientData.clear();
    form->initializeNewPatient();
    form->setIdList(idList);
    patientModule->show();
    requestTableOfScenarios();
}

void CTPatientModuleAdmin::searchResults(QString id_scenario){
    tableOfResults->filterByScenario(id_scenario);
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


/*
 *In case the user has inserted changes to the patient's data in the user interface
 */
void CTPatientModuleAdmin::updateLocalPatientData(QHash<QString,QString> patientDataFromUI){
    foreach(QString key, patientDataFromUI.keys()){
       localPatientData[key] = patientDataFromUI[key];
    }
}


void CTPatientModuleAdmin::showConfirmationMessageStatus(){

    patientModule->showOkMessage("Operation successful!");
}


/*Select on pre-known columns of the table patients*/
void CTPatientModuleAdmin::requestTableOfScenarios()
{
    QStringList fieldNames = QStringList() <<"id" << "execution_day"
                                          << "execution_order"
                                          << "creation_date" << "last_edited"
                                          <<  "description" << "xml_description"
                                           << "flag";

    CTTableRecord rec = CTTableRecord();
    int i =0;
    foreach(QString fieldName, fieldNames)
    {
        rec.insert(i, CTTableField(fieldName, fieldName));
        i++;
    }
    QString stmt = CTQueryParser::xmlStatement(CTQueryParser::SelectStatement,
                                               "test_scenario",rec);
    execParsedQuery(stmt, QString());
}


void CTPatientModuleAdmin::execParsedQuery(QString initStmt, QString whereStmt)
{
    emit requestToWriteIntoSocket(CTQueryParser::prepareQuery(
                                      initStmt,whereStmt), CT_DBSDATA);
}


void CTPatientModuleAdmin::proccessData(QByteArray table_data, QString table_name)
{
    if(table_name == "test_scenario")
        tableOfScenarios->init(CTXmlDataParser::parse_table(table_data));
}


