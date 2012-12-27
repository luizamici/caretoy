#ifndef CT_SQLTABLE_H
#define CT_SQLTABLE_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView>
#include "ct_dbconn.h"
#include "ct_sqlmodel.h"

class CTSqlTable : public QTableView
{
    Q_OBJECT
public:
    explicit CTSqlTable(QWidget *parent = 0);

    CTDBConn *db;
    CTSqlModel *tableModel;

    QHash<QString,QString> getSelected();
    void submitAll();


signals:
    
public slots:
    void save(QHash<QString,QString> scenario);
    
};

#endif // CT_SQLTABLE_H
