#include "ct_tabledata.h"


CTTableData::CTTableData(const QString &tableName)
    :table_name(tableName),
      numRows(0),
      numColumns(0)
{
    data = QList<CTTableRecord>();
}

bool CTTableData::insertRecord(int pos, const CTTableRecord &rec)
{
    if(pos == -1)
    {
        data.append(rec);
        numRows++;
        numColumns = rec.count();
        return false;
    }
    else
        data.insert(pos,rec);
    return true;
}

void CTTableData::remove(int pos)
{
    data.removeAt(pos);
    numRows--;
}

CTTableRecord CTTableData::getRecord(int pos) const
{
    return data.at(pos);
}

/*Returns a record with empty values*/
CTTableRecord CTTableData::record()
{
    if(data.isEmpty())
        return CTTableRecord();
    else
    {
        CTTableRecord rec;
        rec = data.at(0);
        rec.clearValues();
        return rec;
    }
}

void CTTableData::replace(int row, int column, QString value)
{
    CTTableRecord rec = getRecord(row);
    rec.setValue(column,value);
    data.replace(row, rec);
}

