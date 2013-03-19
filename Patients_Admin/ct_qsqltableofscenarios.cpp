#include "ct_qsqltableofscenarios.h"
#include <QDebug>

CTQSqlTableOfScenarios::CTQSqlTableOfScenarios(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent,db)
{
    setTable("scenarios");

    setHeaderData(0, Qt::Horizontal, tr("Description"));
    setHeaderData(1, Qt::Horizontal, tr("Total runtime"));
    setHeaderData(2, Qt::Horizontal, tr("Last editor"));
    setHeaderData(3, Qt::Horizontal, tr("Last edited"));
    setHeaderData(4, Qt::Horizontal, tr("Creation date"));
    setHeaderData(5, Qt::Horizontal, tr("Execution day"));
    setHeaderData(6, Qt::Horizontal, tr("Execution order"));
    setHeaderData(7, Qt::Horizontal, tr("Id"));
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void CTQSqlTableOfScenarios::initialize(QStringList id_scenarios_for_patient){

    QString filter;
    foreach(QString id_scenario, id_scenarios_for_patient){
        filter += "id = " + id_scenario + " or " ;
    }
    filter.remove(filter.length() - 3,3); //removing the last 'or' of the cycle
    select();
    setFilter(filter);
}
