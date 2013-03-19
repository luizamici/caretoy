#ifndef CT_TABLEOFRESULTS_H
#define CT_TABLEOFRESULTS_H

#include <QTableView>
#include <QtGui>
#include "ct_qsqltableofresults.h"

class CTTableOfResults : public QTableView
{
    Q_OBJECT
public:
    explicit CTTableOfResults(QWidget *parent = 0);

    void setQSqlModel(CTQSqlTableOfResults *sqlTableOfResults);
    void filterByScenario(QString id_scenario);

private:
    CTQSqlTableOfResults *localSqlTable;
    QHeaderView *header;
    
signals:
    
public slots:
    
};

#endif // CT_TABLEOFRESULTS_H
