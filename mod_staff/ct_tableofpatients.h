#ifndef CT_TABLEOFPATIENTS_H
#define CT_TABLEOFPATIENTS_H

#include <QTableView>
#include <QMouseEvent>
#include <QAbstractItemView>
#include <QtGui>
#include "CareToy_Admin/ct_dbconn.h"
#include "ct_qsqltableofpatients.h"



class CTTableOfPatients : public QTableView
{
    Q_OBJECT
public:
    explicit CTTableOfPatients(QWidget *parent = 0);

    void initializeTableOfPatients(CTQSqlTableOfPatients *sqlTableModelOfPatients);

    void updateSelectedPatient(QHash<QString,QString> patientEdited);
    int saveNewPatient(QHash<QString,QString> newPatient);
    void deleteSelectedPatient(QHash<QString,QString> patientToDelete);

    QHash<QString,QString> getSelectedPatient();
    QStringList getListOfId();

signals:
    void rowSelected(QModelIndex);
    void tableSelected(bool b);
    void beforeInsert(QSqlRecord& rec);
    
public slots:
    void tableSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void setSearchCriteria(int index);
    void filter(QString input);
    void submitAll();


private:
    CTDBConn *conn;
    QSqlTableModel *sqlTable;
    QSortFilterProxyModel *proxy ;

    int searchCriteria;
};

#endif // CT_TABLEOFPATIENTS_H
