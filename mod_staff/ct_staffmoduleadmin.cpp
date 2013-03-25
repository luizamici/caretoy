#include "ct_staffmoduleadmin.h"
#include "DbTableXML/ct_tablerecord.h"
#include "DbTableXML/ct_queryparser.h"
#include "DbTableXML/ct_xmldataparser.h"
#include "CareToy_Admin/ct_defs.h"

CTStaffModuleAdmin::CTStaffModuleAdmin(QWidget *parent) :
    QWidget(parent)
{
    /*Two submodules*/
//    workLogs = new CTWorklogs();

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
    connect(tableOfPatients,SIGNAL(execParsedQuery(QString,QString)),this,
            SLOT(execParsedQuery(QString,QString)));
    connect(workLogs, SIGNAL(execParsedQuery(QString,QString)),this, SLOT(execParsedQuery(QString,QString)));


    connect(staffModule->editButton,SIGNAL(clicked()),this,SLOT(requestEdit()));
    connect(staffModule->addButton,SIGNAL(clicked()),this, SLOT(requestNew()));
    connect(staffModule,SIGNAL(deleteSelectedPatient()),this,SLOT(deleteSelectedPatient()));

    connect(staffModule->searchPatient,SIGNAL(searchBy(int)),tableOfPatients,SLOT(setSearchCriteria(int)));
    connect(staffModule->searchPatient,SIGNAL(searchFor(QString)),tableOfPatients,SLOT(filter(QString)));

    connect(workLogs, SIGNAL(nothingToSave(QString)),this, SLOT(showMessageStatus(QString)));
}

void CTStaffModuleAdmin::initialize(){

    staffModule->label_1->setText(qApp->property("UserName").toString() + " "
                     + qApp->property("UserSurname").toString());
    staffModule->label_2->setText("<font color= green>" + qApp->property("UserLastLogin").
                     toString() + " </font>");
    requestTable();
    connect(tableOfPatients, SIGNAL(tableInitialized()),this, SLOT(requestWorkLog()));
}


/*Select on pre-known columns of the table patients*/
void CTStaffModuleAdmin::requestTable()
{
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
    execParsedQuery(stmt, QString());
}

void CTStaffModuleAdmin::requestWorkLog()
{
    qDebug() << "Requesting worlogs";
    QStringList fieldNames = QStringList() << "relativetimestamp" << "log" << "id" ;

    CTTableRecord rec = CTTableRecord();
    int i =0;
    foreach(QString fieldName, fieldNames)
    {
        rec.insert(i, CTTableField(fieldName, fieldName));
        i++;
    }
    QString stmt = CTQueryParser::xmlStatement(CTQueryParser::SelectStatement,
                                               "worklogs",rec);
    execParsedQuery(stmt, QString());
}

void CTStaffModuleAdmin::execParsedQuery(QString initStmt, QString whereStmt)
{
    qDebug() << initStmt << whereStmt;
    emit requestToWriteIntoSocket(CTQueryParser::prepareQuery(
                                      initStmt,whereStmt), CT_DBSDATA);
}


void CTStaffModuleAdmin::proccessData(QByteArray table_data, QString table_name)
{
    if(table_name == "patients")
        tableOfPatients->init(CTXmlDataParser::parse_table(table_data));
    else if(table_name == "worklogs")
    {
        qDebug() << table_data;
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

void CTStaffModuleAdmin::showMessageStatus(QString message){
    QPalette palette;
    palette.setColor( QPalette::WindowText, "black" );
    staffModule->statusBar->setPalette( palette );
    staffModule->statusBar->showMessage(message,5000);
}
/*******************************************************************************/



