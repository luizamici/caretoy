#include "ct_sqlmodel.h"

CTSqlModel::CTSqlModel(QWidget *parent, QSqlDatabase db) :
    QSqlTableModel(parent,db)
{
}

/*Making 'setQuery' public*/
void CTSqlModel::setQuery(const QSqlQuery &query)
{
    QSqlQueryModel::setQuery(query);
}
