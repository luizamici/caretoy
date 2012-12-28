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

    void insertIntoTable(QSqlRecord scenarioRecord);
    void copyIntoTable(QSqlRecord scenarioRecord);
    void deleteFromTable(QString id_scenario);
    QHash<QString,QString> getSelected();
    QSqlRecord getSelectedRecord();
    void submitAll();

private:
    QModelIndex getIndex(QString id_scenario);
    
public slots:
    void save(QHash<QString,QString> scenario);
    
};

#endif // CT_SQLTABLE_H
