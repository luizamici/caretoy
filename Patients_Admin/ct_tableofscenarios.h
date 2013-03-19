#ifndef CT_TABLEOFSCENARIOS_H
#define CT_TABLEOFSCENARIOS_H

#include <QTableView>
#include <QAbstractItemView>
#include <QtGui>
#include "ct_qsqltableofscenarios.h"
#include <QSqlRecord>

class CTTableOfScenarios : public QTableView
{
    Q_OBJECT
public:
    explicit CTTableOfScenarios(QWidget *parent = 0);

    void setQSqlModel(CTQSqlTableOfScenarios *sqlTableOfScenarios);

private:
    CTQSqlTableOfScenarios *localSqlTable;
    QHeaderView *header;
    
signals:
    void checkForResults(QString id_scenario);

public slots:
    void scenarioSelectionChanged(const QItemSelection &selectedItem,const QItemSelection &deselectedItem);
};

#endif // CT_TABLEOFSCENARIOS_H
