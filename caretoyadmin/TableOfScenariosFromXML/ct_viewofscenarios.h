#ifndef CT_VIEWOFSCENARIOS_H
#define CT_VIEWOFSCENARIOS_H

#include <QWidget>
#include <QTableView>
#include <QtGui>
#include <QtCore>
#include <QHeaderView>
#include "ct_xmldataparser.h"
#include "ct_tablemodel.h"
#include "ct_proxymodel.h"

class CTViewOfScenarios : public QWidget
{
    Q_OBJECT
public:
    explicit CTViewOfScenarios(QWidget *parent = 0);

    QTableView *table;
    CTTableModel *xmlTable;
    QStatusBar *statusBar;

private:

    QPushButton *edit;
    QPushButton *copy;
    QPushButton *remove;

    QSortFilterProxyModel *filterModel;
//    CTProxyModel *customFilterModel ;
    QHash<QString, QString> getSelected();
    bool tableSelected;
//    bool customModelEnabled;


signals:
    void editScenario();
    void newScenario();

    void exec(QString parsedQuery);
    void execParsedQuery(QString init_stmt,QString where_stmt);
public slots:
    void save(QHash<QString, QString> scenario);
    void init(CTTableData *table_data);

private slots:
    void on_edit_clicked();
    void on_add_clicked();
    void on_copy_clicked();
    void on_remove_clicked();

    void tableSelectionChanged(const QItemSelection &selected,
                               const QItemSelection &deselected);

//    void sortByColumn(int column);

};

#endif // CT_VIEWOFSCENARIOS_H
