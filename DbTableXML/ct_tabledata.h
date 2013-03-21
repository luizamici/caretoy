#ifndef CTROWDATA_H
#define CTROWDATA_H

#include <QString>
#include <QObject>
#include <QtCore>

#include "ct_tablefield.h"
#include "ct_tablerecord.h"


class CTTableData
{
public:
    CTTableData(const QString &tableName);

    QString table_name;
    int numRows;
    int numColumns;

    QList<CTTableRecord> data;
    bool insertRecord(int pos, const CTTableRecord& rec);
    CTTableRecord getRecord(int pos) const;
    CTTableRecord record();
    void replace(int row, int column, QString value);
    void remove(int pos);
    //TODO implement an index of the primary keys

    QList< QPair< QString, CTTableField> > constraints;
};

#endif // CTROWDATA_H
