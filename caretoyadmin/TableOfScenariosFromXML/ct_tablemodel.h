#ifndef CT_XMLTABLEOFSCENARIOS_H
#define CT_XMLTABLEOFSCENARIOS_H

#include <QWidget>
#include <QDebug>
#include <QAbstractTableModel>
#include <QtCore>

#include "ct_queryparser.h"

class CTTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    CTTableModel(CTTableData *table_data, QObject *parent);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    bool insertRows(int position, int rows,
                    const QModelIndex &index=QModelIndex());
    bool removeRows(int position, int rows,
                    const QModelIndex &index=QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role=Qt::EditRole);


    void record(const QModelIndex &index);

    void setHeader(int section,const QVariant &value);
    void copyRecord(const QModelIndex &i);
    void deleteRecord(const QModelIndex &index);
    void save(QHash<QString, QString> record);
    
private:
    CTTableData *p_table_data;

    bool insertRowIntoTable(const CTTableRecord &rec);
    bool updateRowInTable(const CTTableRecord &rec);
    bool deleteRowFromTable(const CTTableRecord &rec);


    QHash<QString,QString> map_to_hash(const CTTableRecord &rec);
    CTTableRecord map_from_hash(QHash<QString,QString> &scenario);

    void exportData(const CTTableRecord &rec);

    QModelIndex getIndex(QString id);
    QVector<QVariant> *headerDt;

signals:
    void execParsedQuery(QString init_stmt, QString where_stmt);


};

#endif // CT_XMLTABLEOFSCENARIOS_H
