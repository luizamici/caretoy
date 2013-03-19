#include "ct_tableofpatients.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>

CTTableOfPatients::CTTableOfPatients(QWidget *parent) :
    QTableView(parent)
{
}

void CTTableOfPatients::initializeTableOfPatients(CTQSqlTableOfPatients *sqlTableModelOfPatients){
    sqlTable = sqlTableModelOfPatients;
    sqlTable->setHeaderData(0, Qt::Horizontal, tr("Ref. Number"));
    sqlTable->setHeaderData(1, Qt::Horizontal, tr("Frist Name"));
    sqlTable->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    sqlTable->setHeaderData(3, Qt::Horizontal, tr("Parent 1"));
    sqlTable->setHeaderData(4, Qt::Horizontal, tr("Parent 2"));
    sqlTable->setHeaderData(5, Qt::Horizontal, tr("Date of birth"));
    sqlTable->setHeaderData(6, Qt::Horizontal, tr("Gender"));
    sqlTable->setHeaderData(7, Qt::Horizontal, tr("Gest. age"));
    sqlTable->setHeaderData(8, Qt::Horizontal, tr("Attendant"));

    proxy = new QSortFilterProxyModel();
    proxy->setDynamicSortFilter(true);
    proxy->setSourceModel(sqlTable);

    this->setModel(proxy);
    /*disabling the editing through the table view*/
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSortingEnabled(true);
    this->horizontalHeader()->resizeSections(QHeaderView :: ResizeToContents);
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->hide();

    /*columns obtained by the DB's table 'patient' which are not wished to be viewd */
    this->setColumnHidden(sqlTable->fieldIndex("address"), true);
    this->setColumnHidden(sqlTable->fieldIndex("zip_code"), true);
    this->setColumnHidden(sqlTable->fieldIndex("city"), true);
    this->setColumnHidden(sqlTable->fieldIndex("phone"), true);
    this->setColumnHidden(sqlTable->fieldIndex("email"), true);
    this->setColumnHidden(sqlTable->fieldIndex("notes"), true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

    /*
     *To respect the foreign key constraint between patient's table and user's table
     *the new patient is inserted to the user's table first.
     */
    connect(sqlTable, SIGNAL(beforeInsert(QSqlRecord&)),this, SIGNAL(beforeInsert(QSqlRecord&)));

    /*
     *Signals are emitted if valid rows are selected from the table
     */
    connect(this->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(tableSelectionChanged(QItemSelection,QItemSelection)));
}

void CTTableOfPatients::tableSelectionChanged(const QItemSelection &selected,
                                              const QItemSelection &deselected){
    if(!selected.indexes().isEmpty()){emit tableSelected(true);}
    else{emit tableSelected(false);}
}


QHash<QString,QString> CTTableOfPatients::getSelectedPatient(){
    /*
     *Getting the selected row's index through the QTableView::currentIndex()
     *function, which returns an internal index,different from the one in the DB
     */
    qDebug() << this->selectionModel()->currentIndex();
    QModelIndex index = currentIndex();

    /*Mapping the current index to the source index*/
    QModelIndex indexOfTable = proxy->mapToSource(index);
    int row = indexOfTable.row();

    /*
     *Getting the selected row of the table model
     */
    QSqlRecord patientRecord = sqlTable->record(row);
    /*
     *Preparing the selected patient's data for then returning it
     */
    QHash<QString,QString> patient;
    patient["row"] = QString::number(row);

    patient["id"] = patientRecord.value("id").toString();
    patient["firstname"] = patientRecord.value("firstname").toString();
    patient["lastname"] = patientRecord.value("lastname").toString();
    patient["parent_1"] = patientRecord.value("parent_1").toString();
    patient["parent_2"] = patientRecord.value("parent_2").toString();
    patient["date_of_birth"] = patientRecord.value("date_of_birth").toString();
    patient["sex"] = patientRecord.value("sex").toString();
    patient["gest_age"] = patientRecord.value("gest_age").toString();
    patient["attendant"] = patientRecord.value("attendant").toString();
    patient["address"] = patientRecord.value("address").toString();
    patient["zip_code"] = patientRecord.value("zip_code").toString();
    patient["city"] = patientRecord.value("city").toString();
    patient["phone"] = patientRecord.value("phone").toString();
    patient["email"] = patientRecord.value("email").toString();
    patient["notes"] = patientRecord.value("notes").toString();

    return patient;
}


void CTTableOfPatients::updateSelectedPatient(QHash<QString, QString> patientEdited){
    /*
     *patient["row"] contains the row number of the record wanted to be changed
     */
    int row = patientEdited["row"].toInt();
    foreach(QString key, patientEdited.keys()){
        int fieldIndex = sqlTable->fieldIndex(key);
        QModelIndex index = sqlTable->index(row, fieldIndex);
        /*
         *The changes are saved locally in the SQL model
         */
        sqlTable->setData(index,patientEdited[key],Qt::EditRole);
    }
}

/*
 *CTTableOfPatients::saveNewPatient
 *returns the row of the inserted patient
 *or -1 in case of failure
 */
int CTTableOfPatients::saveNewPatient(QHash<QString, QString> newPatient){
    int row = -1;
    QSqlRecord newPatientRecord = sqlTable->record();

    foreach(QString key,newPatient.keys()){
        newPatientRecord.setValue(key,newPatient[key]);
    }
    if(sqlTable->insertRecord(sqlTable->rowCount(QModelIndex()),newPatientRecord)){
        row = sqlTable->rowCount(QModelIndex()) - 1;
    }
    return row;
}


void CTTableOfPatients::deleteSelectedPatient(QHash<QString, QString> patientToDelete){

    int row = patientToDelete["row"].toInt();
    if(sqlTable->index(row,0).isValid()){
        sqlTable->removeRows(row ,1);
    }
}


QStringList CTTableOfPatients::getListOfId(){
    QStringList idList;
//    while (sqlTable->canFetchMore())
//         sqlTable->fetchMore();
    for (int i = 0; i < sqlTable->rowCount(); ++i) {
        idList << sqlTable->data(sqlTable->index(i,sqlTable->fieldIndex("id"))).toString();
    }
    return idList;
}

/*This part is used for searching throught the list of patients*/

void CTTableOfPatients::setSearchCriteria(int index)
{ 
    /*
     *maps the indexes of the searchQCB to the indexes of the model
     */
    if(index == 6){
        this->searchCriteria = 7;
    }else{
        this->searchCriteria = index - 1;
    }
}

/*
 *Filters search results
 */
void CTTableOfPatients::filter(QString input){

    proxy->setFilterKeyColumn(searchCriteria);
    proxy->setFilterRegExp(QRegExp(input, Qt::CaseInsensitive));
}

/*****************************************************************/

void CTTableOfPatients::submitAll(){

    sqlTable->database().transaction();

    if(sqlTable->submitAll()){
        sqlTable->database().commit();
        qDebug() << "SUCCESS";}
    else{
        sqlTable->database().rollback();
        qDebug() << "there was an error " << sqlTable->lastError().text();
    }
}
