#ifndef CT_TABLEOFSCENARIOS_H
#define CT_TABLEOFSCENARIOS_H

#include <QTableView>
#include <QAbstractItemView>
#include <QtGui>
#include <QSqlRecord>

#include "DbTableXML/ct_tabledata.h"
#include "DbTableXML/ct_tablemodel.h"

class CTTableOfScenarios : public QTableView
{
    Q_OBJECT
public:
    explicit CTTableOfScenarios(QWidget *parent = 0);

    CTTableModel *xmlTable;
    void init(CTTableData *table_data);


private:
    QSortFilterProxyModel *filterModel;
    
signals:
    void checkForResults(QString id_scenario);

public slots:
    void scenarioSelectionChanged(const QItemSelection &selectedItem,const QItemSelection &deselectedItem);
};

#endif // CT_TABLEOFSCENARIOS_H
