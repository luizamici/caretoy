#include "ct_sqlmodel.h"

CTSqlModel::CTSqlModel(QWidget *parent, QSqlDatabase db) :
    QSqlTableModel(parent,db)
{
}

void CTSqlModel::setQuery(const QSqlQuery &query)
{
    QSqlQueryModel::setQuery(query);
}
