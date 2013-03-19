#include "ct_qsqltableofresults.h"


CTQSqlTableOfResults::CTQSqlTableOfResults(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent,db)
{
    setTable("scenarios_result");

    setHeaderData(0, Qt::Horizontal, tr("Total runtime"));
    setHeaderData(1, Qt::Horizontal, tr("Execution date"));
    setHeaderData(2, Qt::Horizontal, tr("Associated scenario"));

    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void CTQSqlTableOfResults::initialize(QStringList id_scenarios_for_patient){

    QString filter;
    foreach(QString id_scenario, id_scenarios_for_patient){
        filter += "associated_scenario = " + id_scenario + " or " ;
    }
    filter.remove(filter.length() - 3,3); //removing the last 'or' of the cycle
    select();
    setFilter(filter);
}
