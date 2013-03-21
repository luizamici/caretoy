#include "ct_tableofscenarios.h"



CTTableOfScenarios::CTTableOfScenarios(QWidget *parent) :
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


void CTTableOfScenarios::init(CTTableData *table_data)
{
    xmlTable = new CTTableModel(table_data,this->parentWidget());
    connect(xmlTable, SIGNAL(execParsedQuery(QString,QString)),this,
            SIGNAL(execParsedQuery(QString,QString)));
    filterModel = new QSortFilterProxyModel();
    filterModel->setSourceModel(xmlTable);
    this->setModel(filterModel);


    xmlTable->setHeader(0, "ID");
    xmlTable->setHeader(1,"Execution day");
    xmlTable->setHeader(2,"Execution order");
    xmlTable->setHeader(3,"Creation date");
    xmlTable->setHeader(4, "Last edited");
    xmlTable->setHeader(5, "Description");


    /*columns which are not wished to be viewd */
    this->setColumnHidden(6, true); // xml_description hidden
    this->setColumnHidden(7, true);

    /*
     *Connection of this signal is done after the setModel() otherwise selectionModel() is null
     */
    connect(this->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(scenarioSelectionChanged(QItemSelection,QItemSelection)));
}


/*
 *It firstly checks that the selection is not out of the table of scenarios
 *and then emits a signal checkForResults() with the extracted id of the
 *selected scenario
 */
void CTTableOfScenarios::scenarioSelectionChanged(const QItemSelection &selectedItem,
                                                  const QItemSelection &deselectedItem){
//    QModelIndex index ;
//    if(!selectedItem.isEmpty())
//        index = selectedItem.indexes().at(0);

//    if(index.isValid()){
//        int row = index.row();
//        QSqlRecord patientRecord = localSqlTable->record(row);
//        QString id_scenario = patientRecord.value("id").toString();
//        emit checkForResults(id_scenario);
//    }
}

