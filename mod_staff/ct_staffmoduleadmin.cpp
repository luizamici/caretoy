#include "ct_staffmoduleadmin.h"
#include "DbTableXML/ct_tablerecord.h"
#include "DbTableXML/ct_queryparser.h"
#include "DbTableXML/ct_xmldataparser.h"
#include "CareToy_Admin/ct_defs.h"

CTStaffModuleAdmin::CTStaffModuleAdmin(QWidget *parent) :
    QWidget(parent)
{
    /*Two submodules*/
    workLogs = new CTWorklogsWidget();
    tableOfPatients = new CTTableOfPatients(this->parentWidget());

    staffModule = new CTStaffModule(this->parentWidget());
    staffModule->subLayout3->addWidget(workLogs);
    staffModule->subLayout2->addWidget(tableOfPatients,1,0);

    /*
     *Connections betwwen the tableOfPatients submodule and the staffModule interface,
     *for enabling/disabling the edit/delete button in case of selection-or-not of a
     *patient from the list
     */
    connect(tableOfPatients,SIGNAL(tableSelected(bool)),staffModule->deleteButton,
            SLOT(setEnabled(bool)));
    connect(tableOfPatients,SIGNAL(tableSelected(bool)),staffModule->editButton,
            SLOT(setEnabled(bool)));
    connect(tableOfPatients,SIGNAL(execParsedQuery(QString,QString,Data)),this,
            SLOT(execParsedQuery(QString,QString,Data)));
    connect(workLogs, SIGNAL(execParsedQuery(QString,QString,Data)),this, SLOT(
                execParsedQuery(QString,QString,Data)));

    connect(tableOfPatients, SIGNAL(tableInitialized()),this, SLOT(
                afterTableInit()));

    connect(staffModule->editButton,SIGNAL(clicked()),this,SLOT(requestEdit()));
    connect(staffModule->addButton,SIGNAL(clicked()),this, SLOT(requestNew()));
    connect(staffModule,SIGNAL(deleteSelectedPatient()),this,SLOT(deleteSelectedPatient()));

    connect(staffModule->searchPatient,SIGNAL(searchBy(int)),tableOfPatients,SLOT(setSearchCriteria(int)));
    connect(staffModule->searchPatient,SIGNAL(searchFor(QString)),tableOfPatients,SLOT(filter(QString)));

    connect(workLogs, SIGNAL(nothingToSave(QString)),this, SLOT(showMessageStatus(QString)));
}

void CTStaffModuleAdmin::initialize(){

    qDebug() << "CTStaffModuleAdmin::initialize";
    staffModule->label_1->setText(qApp->property("UserName").toString() + " "
                     + qApp->property("UserSurname").toString());
    staffModule->label_2->setText("<font color= green>" + tr("Last login: ") + qApp->property("UserLastLogin").
                     toString() + " </font>");
    requestPatientsTable();
    staffModule->showMessage("Retreiving data from the DB...");
}

void CTStaffModuleAdmin::refresh()
{
    if(qApp->property("dirty_data") == T_Patient)
        requestPatientsTable();
    else if(qApp->property("dirty_data") == T_Worklogs)
        requestWorkLog();
}

void CTStaffModuleAdmin::afterTableInit()
{
    if(qApp->property("dirty_data") == T_NoOne)
        requestWorkLog();
}

/*Select on pre-known columns of the table patients*/
void CTStaffModuleAdmin::requestPatientsTable()
{
    qDebug() << "CTStaffModuleAdmin::requestTable()";
    QStringList fieldNames = QStringList() <<"id" << "firstname"
                                          << "lastname"
                                          << "parent_1" << "parent_2"
                                          <<  "date_of_birth" << "sex"
                                          << "gest_age" << "attendant"
                                          << "address" << "zip_code"
                                          << "city" << "phone" << "email"
                                          << "notes";

    CTTableRecord rec = CTTableRecord();
    int i =0;
    foreach(QString fieldName, fieldNames)
    {
        rec.insert(i, CTTableField(fieldName, fieldName));
        i++;
    }
    QString stmt = CTQueryParser::xmlStatement(CTQueryParser::SelectStatement,
                                               "patients",rec);
    execParsedQuery(stmt, QString(), T_NoOne);
}

void CTStaffModuleAdmin::requestWorkLog()
{
    qDebug() << "CTStaffModuleAdmin::requestWorkLog";
    QStringList fieldNames = QStringList() << "relativetimestamp" << "log" << "id" ;

    CTTableRecord rec = CTTableRecord();
    int i =0;
    foreach(QString fieldName, fieldNames)
    {
        rec.insert(i, CTTableField(fieldName, fieldName));
        i++;
    }
    QString init_stmt = CTQueryParser::xmlStatement(CTQueryParser::SelectStatement,
                                               "worklogs",rec);

    rec.clear();
    CTTableField field;
    field.setName("staff");
    field.setValue(qApp->property("UserID").toString());
    rec.append(field);

    QString where_stmt = CTQueryParser::xmlStatement(CTQueryParser::WhereStatement,
                                                     "worklogs",rec);
    execParsedQuery(init_stmt, where_stmt, T_NoOne);
}

void CTStaffModuleAdmin::execParsedQuery(QString initStmt, QString whereStmt, Data type_of_data)
{
    qDebug() << "CTStaffModuleAdmin::execParsedQuery" ;
    if(type_of_data == T_Patient)
        qDebug( ) << "Setting patient data as dirty";
    else if(type_of_data == T_Worklogs)
        qDebug() << "Setting worklogs data as dirty";
    if(type_of_data != T_NoOne)
        qApp->setProperty("dirty_data", type_of_data);
    emit requestToWriteIntoSocket(CTQueryParser::prepareQuery(
                                      initStmt,whereStmt), CT_DBSDATA);
}


void CTStaffModuleAdmin::proccessData(QByteArray table_data, QString table_name)
{
    if(table_name == "patients")
        tableOfPatients->init(CTXmlDataParser::parse_table(table_data));
    else if(table_name == "worklogs")
    {
        workLogs->init(table_data);
    }
}

void CTStaffModuleAdmin::showStaffModule(){
    staffModule->show();
}


/********************Patient management**************************************/
void CTStaffModuleAdmin::requestEdit(){
    emit editSelectedPatient(getSelectedPatient());
}

void CTStaffModuleAdmin::requestNew(){
    QStringList idList = tableOfPatients->getListOfId();
    emit openNewPatientDialog(idList);
}

void CTStaffModuleAdmin::deleteSelectedPatient(){
    QHash<QString,QString> patientToDelete = getSelectedPatient();
    tableOfPatients->deleteSelectedPatient(patientToDelete);
}

QHash<QString,QString> CTStaffModuleAdmin::getSelectedPatient(){
    return(tableOfPatients->getSelectedPatient());
}

void CTStaffModuleAdmin::updateSelectedPatient(QHash<QString, QString> patientEdited){
    tableOfPatients->updateSelectedPatient(patientEdited);
}

void CTStaffModuleAdmin::saveNewPatient(QHash<QString, QString> newPatient){
   tableOfPatients->saveNewPatient(newPatient);
}

/*******************************************************************************/



/************************Status bar messages************************************/
void CTStaffModuleAdmin::showConfirmationMessageStatus(){

    staffModule->showOkMessage("Operation successful!");
}

void CTStaffModuleAdmin::showFailureMessageStatus()
{
    staffModule->showFailureMessage("Operation failed!");
}

void CTStaffModuleAdmin::showWarningMessage(QString mssg)
{
    staffModule->showMessage(mssg);
}

void CTStaffModuleAdmin::showMessageStatus(QString message){
    QPalette palette;
    palette.setColor( QPalette::WindowText, "black" );
    staffModule->statusBar->setPalette( palette );
    staffModule->statusBar->showMessage(message,5000);
}

/*******************************************************************************/



