#include "ct_staffmoduleadmin.h"
#include "ct_worklogs.h"
#include "DbTableXML/ct_tablerecord.h"
#include "DbTableXML/ct_queryparser.h"
#include "DbTableXML/ct_xmldataparser.h"
#include "CareToy_Admin/ct_defs.h"

CTStaffModuleAdmin::CTStaffModuleAdmin(QWidget *parent) :
    QWidget(parent)
{
    /*Two submodules*/
    workLogs = new CTWorklogs();
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


    connect(staffModule->editButton,SIGNAL(clicked()),this,SLOT(requestEdit()));
    connect(staffModule->addButton,SIGNAL(clicked()),this, SLOT(requestNew()));
    connect(staffModule,SIGNAL(deleteSelectedPatient()),this,SLOT(deleteSelectedPatient()));

    connect(staffModule->searchPatient,SIGNAL(searchBy(int)),tableOfPatients,SLOT(setSearchCriteria(int)));
    connect(staffModule->searchPatient,SIGNAL(searchFor(QString)),tableOfPatients,SLOT(filter(QString)));

    connect(workLogs,SIGNAL(saveLog()),this,SLOT(getLog()));
    connect(workLogs->comboBox,SIGNAL(currentIndexChanged(QString)),this, SLOT(updateWorklogEditor(QString)));
}

void CTStaffModuleAdmin::initialize(){

    staffModule->label_1->setText(qApp->property("UserName").toString() + " "
                     + qApp->property("UserSurname").toString());
    staffModule->label_2->setText("<font color= green>" + qApp->property("UserLastLogin").
                     toString() + " </font>");
    requestTable();
    requestWorkLog();
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
    //TODO
}

void CTStaffModuleAdmin::execParsedQuery(QString initStmt, QString whereStmt)
{
    qDebug() << initStmt << whereStmt;
    emit requestToWriteIntoSocket(CTQueryParser::prepareQuery(
                                      initStmt,whereStmt), CT_DBSDATA);
}


void CTStaffModuleAdmin::proccessData(QByteArray table_data)
{
    tableOfPatients->init(CTXmlDataParser::parse_table(table_data));
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

/**********************************Worklog management*******************************************/
void CTStaffModuleAdmin::setWorkLogList(QMap<QPair<QString,QString>,QString> workLogsListFromDB){
    /*
     *This hash map has the worklogs' timestamps in the keys, and the relative
     *log as value.
    */
    localWorkLogsList = workLogsListFromDB;
    /*
     *Since the QMap doesn't keep in memory the order of insert, a sorting of
     *the worklog list by id_worklog is necessary
     */
    sortLocalWorkLogsListById();
    workLogs->initializeComboBox(timestamps);
    workLogs->initializeWorklogList(timestamps,logs);
}

void CTStaffModuleAdmin::refreshWorkLogList(QMap<QPair<QString, QString>, QString> workLogsListFromDB,bool newLog){
    /*
     *This hash map has the worklogs' timestamps in the keys, and the relative
     *log as value.
    */
    localWorkLogsList = workLogsListFromDB;
    sortLocalWorkLogsListById();
    if(!newLog){
        workLogs->initializeWorklogList(timestamps,logs);
    }else{
        workLogs->initializeWorklogList(timestamps,logs);
        workLogs->initializeComboBox(timestamps);
        workLogs->comboBox->setCurrentIndex(workLogs->comboBox->count() - 1);
    }
}

void CTStaffModuleAdmin::getLog(){
    /*
     *This public slot is used in both cases: when a new log is requested to be saved
     *and when an already existing log is being changed and submitted
     */
    QHash<QString,QString> newWorklogToSave;
    QHash<QString,QString> worklogToUpdate;
    if(isNewLog()){
        /*If the worklog being saved to the DB is new, then its corresponding
         *timestamp is created new
         */
        newWorklogToSave["log"] = workLogs->getLog();
        QDateTime currentDateTime = QDateTime::currentDateTime();
        newWorklogToSave["relativetimestamp"] = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        emit saveNewLog(newWorklogToSave);
    }else{
        if(!localSelectedLogTimestamp.isEmpty()){
            QPair<QString,QString> key;
            for(int i = 0; i < localWorkLogsList.keys().size() ; i++){
                key = localWorkLogsList.keys().at(i);
                if(key.second == localSelectedLogTimestamp){
                    worklogToUpdate["id_worklog"] = key.first;
                    worklogToUpdate["relativetimestamp"] = localSelectedLogTimestamp;
                    worklogToUpdate["log"] = workLogs->getLog();
                }
            }
            emit updateLog(worklogToUpdate);
        }else{qDebug() << "CTStaffModuleAdmin::getLog() is returning with error because localSelectedLogTimestamp is empty ";}
    }
}

void CTStaffModuleAdmin::updateWorklogEditor(QString selectedlogtimestamp){
    /*
     *Since the selected log is probably being further changed,
     *its corresponging timestamps is stored locally for then being
     *used when changes are requested to be saved
     */
    QPair<QString,QString> key;
    localSelectedLogTimestamp = selectedlogtimestamp;
    for(int i = 0; i < localWorkLogsList.keys().size() ; i++){
        key = localWorkLogsList.keys().at(i);
        if(key.second == localSelectedLogTimestamp){
            workLogs->workLogEdit->setText(localWorkLogsList[key]);
            workLogs->disableWorkLogEditor(workLogs->workLogEdit);
            if(!isNewLog())showMessageStatus("Press 'Edit' button for changing the selected worklog");
        }
    }

}

bool CTStaffModuleAdmin::isNewLog(){
    return(workLogs->comboBox->currentIndex() == 0);
}

void CTStaffModuleAdmin::sortLocalWorkLogsListById(){

    QPair<QString,QString> key;
    QList<int> id_worklogs;
    timestamps.clear();
    logs.clear();
    /*populating the list of worklogs from the localWorkLogsList*/
    for(int i = 0; i < localWorkLogsList.keys().size() ; i ++){
        key = localWorkLogsList.keys().at(i);
        id_worklogs.append(key.first.toInt());
    }
    /*sorting the id's list in ascending order*/
    qSort(id_worklogs.begin(), id_worklogs.end(), qLess<int>());
    /*
     *The sorting does not return another QMap object since that brings us
     *to nothing. Instead the sorted data is appended to two lists:
     *list of timestamps and list of logs
     */
    foreach(int id_worklog, id_worklogs){
        for(int i = 0; i < localWorkLogsList.keys().size() ; i ++){
            key = localWorkLogsList.keys().at(i);
            if(id_worklog == key.first.toInt()){
                timestamps.append(key.second);
                logs.append(localWorkLogsList[key]);
            }
        }
    }
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



