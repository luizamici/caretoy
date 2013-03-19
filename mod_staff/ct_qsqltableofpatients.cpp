#include "ct_qsqltableofpatients.h"


CTQSqlTableOfPatients::CTQSqlTableOfPatients(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent,db)
{
    setTable("patients");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
}

