#include "ct_tableofresults.h"

CTTableOfResults::CTTableOfResults(QWidget *parent) :
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

void CTTableOfResults::setQSqlModel(CTQSqlTableOfResults *sqlTableOfResults){

    localSqlTable = new CTQSqlTableOfResults();
    localSqlTable = sqlTableOfResults;
    setModel(localSqlTable);
    /*
     *Remember to take care of this later since the model has to
     *be independent of the underlying db
     */
    header->moveSection(2,0);
    header->moveSection(1,2);
    /****************************************/
    this->setColumnHidden(localSqlTable->fieldIndex("id_result"),true);
}

/*
 *Shows only the results associated to the chosen scenario
 */
void CTTableOfResults::filterByScenario(QString id_scenario){

    QSortFilterProxyModel *proxy = new QSortFilterProxyModel();
    proxy->setSourceModel(localSqlTable);
    int column = localSqlTable->fieldIndex("associated_scenario");
    proxy->setFilterKeyColumn(column);
    if(id_scenario.isNull() || id_scenario.isEmpty()){
        qDebug() << "Nothing selected";
    }
    else{
        proxy->setFilterRegExp(QRegExp(id_scenario, Qt::CaseInsensitive));
        setModel(proxy);
    }
}

