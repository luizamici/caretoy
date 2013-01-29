#ifndef CT_SQLTABLE_H
#define CT_SQLTABLE_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView>
#include "ct_dbconn.h"
#include "ct_sqlmodel.h"

//#include "ct_xmltableofscenarios.h"

class CTSqlTable : public QTableView
{
    Q_OBJECT
public:
    explicit CTSqlTable(QWidget *parent = 0);

    CTDBConn *db;
    CTSqlModel *tableModel;
//    CTXMLTableOfScenarios *xmlTableModel;

    void insertIntoTable(QSqlRecord scenarioRecord);
    void copyIntoTable(QSqlRecord scenarioRecord);
    void deleteFromTable(QString id_scenario);
    bool save(QHash<QString,QString> scenario);
    QHash<QString,QString> getSelected();
    QSqlRecord getSelectedRecord();
    bool submitAll();

private:
    QModelIndex getIndex(QString id_scenario);
    Log4Qt::Logger *p_logger;
signals:
    void success();
    void error(QString errMessage);
    
};

#endif // CT_SQLTABLE_H
