#ifndef CT_SQLTABLEOFSCENARIOS_H
#define CT_SQLTABLEOFSCENARIOS_H

#include <QTableView>
#include <QSqlTableModel>
#include "ct_dbconn.h"

class CTSqlTableOfScenarios : QTableView
{
public:
    CTSqlTableOfScenarios(QWidget *parent = 0);

    CTDBConn *db;
    QSqlTableModel *tableModel;
};

#endif // CT_SQLTABLEOFSCENARIOS_H
