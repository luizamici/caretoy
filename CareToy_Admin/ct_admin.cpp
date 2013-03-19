#include "ct_admin.h"
#include "mod_staff/ct_staffmodule.h"
#include "mod_staff/ct_qsqltableofpatients.h"


CTScenariosAdmin::CTScenariosAdmin(QObject *parent) :
    QObject(parent)
{
    dbConnAdmin = new CTDBConnAdmin();
    loginAdmin = new CTLoginAdmin();
    staffModuleAdmin = new CTStaffModuleAdmin();
    patientModuleAdmin = new CTPatientModuleAdmin();

   /*
    *This signal is emitted whenever the user presses 'ok' button on the login dialog
    *The dbConnAdmin controls if the username and password inserted are present in the db
    */
    connect(loginAdmin,SIGNAL(requestForAuthentication(QString,QString)),dbConnAdmin,SLOT(authenticate(QString,QString)));
   /*
    *The DB connection emits this signal whenever the inserted credentials were not in the db
    *In this case a message is shown in the login dialog's status bar
    */
    connect(dbConnAdmin,SIGNAL(authorizationFailed()),loginAdmin,SLOT(showWrongCredentialsMessage()));
   /*
    *In case of successful authentication the login dialog is closed and the staff module ui is shown
    */
    connect(dbConnAdmin,SIGNAL(userAuthorized(QHash<QString,QString>)),this, SLOT(loginSuccessful(QHash<QString,QString>)));
    connect(dbConnAdmin,SIGNAL(refreshStaffModule(QString)),this, SLOT(refreshStaffModule(QString)));

    /*
     *In case the staffModuleAdmin is requesting a new worklog to be stored to the DB
     */
    connect(staffModuleAdmin,SIGNAL(saveNewLog(QHash<QString,QString>)),dbConnAdmin,SLOT(saveNewLog(QHash<QString,QString>)));
    connect(staffModuleAdmin,SIGNAL(updateLog(QHash<QString,QString>)),dbConnAdmin,SLOT(updateLog(QHash<QString,QString>)));
    connect(staffModuleAdmin,SIGNAL(editSelectedPatient(QHash<QString,QString>)),this,SLOT(editSelectedPatient(QHash<QString,QString>)));
    connect(staffModuleAdmin,SIGNAL(openNewPatientDialog(QStringList)),patientModuleAdmin,SLOT(openNewPatientDialog(QStringList)));

    connect(patientModuleAdmin,SIGNAL(selectedPatientChanged(QHash<QString,QString>)),staffModuleAdmin,SLOT(updateSelectedPatient(QHash<QString,QString>)));
    connect(patientModuleAdmin,SIGNAL(newPatientAdded(QHash<QString,QString>)),staffModuleAdmin,SLOT(saveNewPatient(QHash<QString,QString>)));

    connect(staffModuleAdmin,SIGNAL(error(QString)),patientModuleAdmin,SLOT(showErrorMessage(QString)));
    connect(staffModuleAdmin,SIGNAL(success(QString,QString)),patientModuleAdmin,SLOT(updateRow(QString,QString)));
    connect(staffModuleAdmin,SIGNAL(insertUser(QSqlRecord&)),dbConnAdmin, SLOT(insertUser(QSqlRecord&)));

}

void CTScenariosAdmin::editSelectedPatient(QHash<QString, QString> selectedPatient){

    QStringList scenarios_for_patient = dbConnAdmin->getScenariosForPatient(selectedPatient["id"]);
    sqlTableOfScenarios = new CTQSqlTableOfScenarios(this, dbConnAdmin->getDBConnection());
    sqlTableOfResults = new CTQSqlTableOfResults(this, dbConnAdmin->getDBConnection());
    patientModuleAdmin->editSelectedPatient(selectedPatient,sqlTableOfScenarios,sqlTableOfResults);
    if (!scenarios_for_patient.isEmpty()){
        sqlTableOfScenarios->initialize(scenarios_for_patient);
        sqlTableOfResults->initialize(scenarios_for_patient);
    }
}

/*
 *The user interface switches from the login dialog to the staff module window in case of successful login
 */
void CTScenariosAdmin::loginSuccessful(QHash<QString,QString> userData){

    loginAdmin->close();
    userDataSession = userData; // id_staff,name,lastname
    initializeStaffModule();
}

void CTScenariosAdmin::initializeStaffModule(){

    staffModuleAdmin->initialize(userDataSession);

    sqlTableOfPatient = new CTQSqlTableOfPatients(this, dbConnAdmin->getDBConnection());
    staffModuleAdmin->setSqlTableModelOfPatients(sqlTableOfPatient);

    staffModuleAdmin->setWorkLogList(dbConnAdmin->getWorkLogs(userDataSession["id_staff"]));
    staffModuleAdmin->showStaffModule();
}

void CTScenariosAdmin::refreshStaffModule(QString submodule){

    CTQSqlTableOfPatients *sqlTable;
    if( submodule == "all"){
        sqlTable = new CTQSqlTableOfPatients(this,dbConnAdmin->getDBConnection());
        sqlTable->setTable("patients");
        sqlTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
        sqlTable->select();
        staffModuleAdmin->setSqlTableModelOfPatients(sqlTable);
        staffModuleAdmin->setWorkLogList(dbConnAdmin->getWorkLogs(userDataSession["id_staff"]));
    }else if( submodule == "tableOfPatients"){
        sqlTable = new CTQSqlTableOfPatients(this,dbConnAdmin->getDBConnection());
        sqlTable->setTable("patients");
        sqlTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
        sqlTable->select();
        staffModuleAdmin->setSqlTableModelOfPatients(sqlTable);
    }else if( submodule == "updatedworklog"){
        staffModuleAdmin->refreshWorkLogList(dbConnAdmin->getWorkLogs(userDataSession["id_staff"]),false);
    }else if( submodule == "insertednewworklog"){
        staffModuleAdmin->refreshWorkLogList(dbConnAdmin->getWorkLogs(userDataSession["id_staff"]),true);
    }
    staffModuleAdmin->showConfirmationMessageStatus();
}

