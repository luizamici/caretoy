#ifndef CT_XMLTABLEOFSCENARIOS_H
#define CT_XMLTABLEOFSCENARIOS_H

#include <QWidget>
#include <QDebug>
#include <QAbstractTableModel>

#include "ct_queryparser.h"
#include "ct_xmldataparser.h"

class CTTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    CTTableModel(CTTableData* table_data, QObject *parent);

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

    QHash<QString, QString> record(const QModelIndex &index);
    void copyRecord(const QModelIndex &i);

    void deleteRecord(const QModelIndex &index);
    void save(QHash<QString, QString> record_hash);
    
private:
    CTTableData *p_table_data;

    bool insertRowIntoTable(const QHash<QString, QString> &values);

    QStringList map(QHash<QString,QString> record_hash);
    QHash<QString,QString> mapToHash(QStringList record);
    QModelIndex getIndex(QString id);


};

#endif // CT_XMLTABLEOFSCENARIOS_H
