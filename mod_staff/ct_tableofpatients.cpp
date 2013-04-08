#include "ct_tableofpatients.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>


CTTableOfPatients::CTTableOfPatients(QWidget *parent) :
    QTableView(parent)
{
    /*disabling the editing through the table view*/
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSortingEnabled(true);
    this->horizontalHeader()->resizeSections(QHeaderView :: ResizeToContents);
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->hide();
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CTTableOfPatients::init(CTTableData *table_data)
{
    xmlTable = new CTTableModel(table_data,this->parentWidget());
    connect(xmlTable, SIGNAL(execParsedQuery(QString,QString)),this,
            SIGNAL(execParsedQuery(QString,QString)));
    filterModel = new QSortFilterProxyModel();
    filterModel->setSourceModel(xmlTable);
    this->setModel(filterModel);

    xmlTable->setHeader(0, tr("Ref. Number"));
    xmlTable->setHeader(1, tr("First Name"));
    xmlTable->setHeader(2, tr("Last Name"));
    xmlTable->setHeader(3, tr("Parent 1"));
    xmlTable->setHeader(4, tr("Parent 2"));
    xmlTable->setHeader(5, tr("Date of birth"));
    xmlTable->setHeader(6, tr("Gender"));
    xmlTable->setHeader(7, tr("Gest. age"));
    xmlTable->setHeader(8, tr("Attendant"));

    /*columns which are not wished to be viewd */
    this->setColumnHidden(9, true);
    this->setColumnHidden(10, true);
    this->setColumnHidden(11, true);
    this->setColumnHidden(12, true);
    this->setColumnHidden(13, true);
    this->setColumnHidden(14, true);
    this->setColumnHidden(15, true);

    /*
     *Signals are emitted if valid rows are selected from the table
     */
    connect(this->selectionModel(),SIGNAL(
                selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(tableSelectionChanged(QItemSelection,QItemSelection)));

    emit tableInitialized();
}

void CTTableOfPatients::tableSelectionChanged(const QItemSelection &selected,
                                              const QItemSelection &deselected){
    /*Checks if any rows on the table have been selected*/
    if(!selected.indexes().isEmpty()){
        table_selected = true;
        emit tableSelected(true);
    }
    else{
        table_selected = false;
        emit tableSelected(false);
    }
}

QHash<QString,QString> CTTableOfPatients::getSelectedPatient(){

    QModelIndex index = filterModel->mapToSource(this->currentIndex());
    return xmlTable->record(index);
}


void CTTableOfPatients::updateSelectedPatient(QHash<QString, QString> patientEdited){
    xmlTable->save(patientEdited, false);
}


void CTTableOfPatients::saveNewPatient(QHash<QString, QString> newPatient){
    xmlTable->save(newPatient, true);
    return ;
}


void CTTableOfPatients::deleteSelectedPatient(QHash<QString, QString> patientToDelete){

    QModelIndex index = filterModel->mapToSource(this->currentIndex());
    xmlTable->deleteRecord(index);
}


QStringList CTTableOfPatients::getListOfRefKeys(){
    return xmlTable->getListOfRefKeys();
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

    filterModel->setFilterKeyColumn(searchCriteria);
    filterModel->setFilterRegExp(QRegExp(input, Qt::CaseInsensitive));
}

/*****************************************************************/

