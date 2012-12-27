#include "ct_sqltableofscenarios.h"


CTSqlTableOfScenarios::CTSqlTableOfScenarios(QWidget *parent)
    :QTableView(parent)
{
    db = new CTDBConn();
    tableModel = new QSqlTableModel(this->parentWidget(),db);
    tableModel->setTable("test_scenario");
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->select();

    tableModel->setHeaderData(1,Qt::Horizontal,tr("ID"));
    tableModel->setHeaderData(2,Qt::Horizontal,tr("Execution day"));
    tableModel->setHeaderData(3,Qt::Horizontal,tr("Execution order"));
    tableModel->setHeaderData(4,Qt::Horizontal,tr("Creation date"));
    tableModel->setHeaderData(5,Qt::Horizontal,tr("Last edited"));

    this->setModel(tableModel);

    /*disabling the editing through the table view*/
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSortingEnabled(true);
    this->horizontalHeader()->resizeSections(QHeaderView :: ResizeToContents);
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->hide();

    /*columns obtained by the DB's table 'patient' which are not wished to be viewd */
    this->setColumnHidden(tableModel->fieldIndex("xml_description"), true);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
}
