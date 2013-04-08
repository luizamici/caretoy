#ifndef CT_TABLEOFPATIENTS_H
#define CT_TABLEOFPATIENTS_H

#include <QTableView>
#include <QMouseEvent>
#include <QAbstractItemView>
#include <QtGui>
#include "CareToy_Admin/ct_defs.h"
#include "DbTableXML/ct_tablemodel.h"



class CTTableOfPatients : public QTableView
{
    Q_OBJECT
public:
    explicit CTTableOfPatients(QWidget *parent = 0);
    void updateSelectedPatient(QHash<QString,QString> patientEdited);
    void saveNewPatient(QHash<QString,QString> newPatient);
    void deleteSelectedPatient(QHash<QString,QString> patientToDelete);
    QHash<QString,QString> getSelectedPatient();
    QStringList getListOfRefKeys();


    void init(CTTableData *table_data);
    CTTableModel *xmlTable;

signals:
    void rowSelected(QModelIndex);
    void tableSelected(bool b);


    void execParsedQuery(QString init_stmt,QString where_stmt, Data type_of_data = T_Patient);
    void tableInitialized();
    
public slots:
    void tableSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void setSearchCriteria(int index);
    void filter(QString input);

private:
    QSortFilterProxyModel *proxy ;
    int searchCriteria;

    QSortFilterProxyModel *filterModel;
    bool table_selected;

};

#endif // CT_TABLEOFPATIENTS_H
