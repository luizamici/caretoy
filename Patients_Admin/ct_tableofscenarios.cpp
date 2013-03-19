#include "ct_tableofscenarios.h"


CTTableOfScenarios::CTTableOfScenarios(QWidget *parent) :
    QTableView(parent)
{ 

    /*disabling the editing through the table view*/
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setSortingEnabled(true);

    header = this->horizontalHeader();
    header->resizeSections(QHeaderView::ResizeToContents);
    header->setResizeMode(QHeaderView::Stretch);

    this->verticalHeader()->hide();
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

}

void CTTableOfScenarios::setQSqlModel(CTQSqlTableOfScenarios *sqlTableOfScenarios){

    localSqlTable = new CTQSqlTableOfScenarios();
    localSqlTable = sqlTableOfScenarios;
    setModel(localSqlTable);

    header->moveSection(7,0);

    //The xml_description is used for further editing ecc.
    this->setColumnHidden(localSqlTable->fieldIndex("xml_description"),true);

    /*Connection of this signal is done after the setModel() otherwise selectionModel() is null*/
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
    QModelIndex index ;
    if(!selectedItem.isEmpty())
        index = selectedItem.indexes().at(0);

    if(index.isValid()){
        int row = index.row();
        QSqlRecord patientRecord = localSqlTable->record(row);
        QString id_scenario = patientRecord.value("id").toString();
        emit checkForResults(id_scenario);
    }
}

