#include "ct_tablemodel.h"
#include "ct_queryparser.h"
#include "ct_tabledata.h"


CTTableModel::CTTableModel(CTTableData* table_data, QObject *parent)
    : QAbstractTableModel(parent)
{
    p_table_data = table_data;
    headerDt = new QVector<QVariant>();
}

int CTTableModel::rowCount(
        const QModelIndex& parent = QModelIndex()) const
{
    return p_table_data->numRows;
}

int CTTableModel::columnCount(
        const QModelIndex& parent = QModelIndex()) const
{
    return p_table_data->numColumns;
}

QVariant CTTableModel::data(
        const QModelIndex& index, int role = Qt::DisplayRole) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    if (index.row() >= rowCount() || index.row() < 0)
        return QVariant();

    return p_table_data->getRecord(index.row()).value(index.column());
}


QVariant CTTableModel::headerData(
        int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if(section < p_table_data->numColumns )
        {
            if(headerDt->isEmpty() || section >= headerDt->size())
                return p_table_data->record().fieldName(section);
            else if(section < headerDt->size())
                return headerDt->at(section);
        }
    }
    else
    {
        return QVariant();
    }
}

void CTTableModel::setHeader(int section, const QVariant &value)
{
    headerDt->insert(section,value);
}

bool CTTableModel::insertRows(int position, int rows,
                                       const QModelIndex &index)
{
    Q_UNUSED(position);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    p_table_data->insertRecord(-1,p_table_data->record());
    endInsertRows();
    return true;
}

bool CTTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        deleteRowFromTable(p_table_data->getRecord(position));
        p_table_data->remove(position);
    }
    endRemoveRows();
    return true;
}

bool CTTableModel::setData(const QModelIndex &index,
                                    const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        p_table_data->replace(index.row(), index.column(), value.toString());

        emit(dataChanged(index, index));
        return true;
    }
    return false;
}


bool CTTableModel::insertRowIntoTable(const CTTableRecord &rec)
{
    QString initial_statement = CTQueryParser::xmlStatement(
                CTQueryParser::InsertStatement, p_table_data->table_name,
                rec);
    execParsedQuery(initial_statement, QString());
    return true;
}


bool CTTableModel::updateRowInTable(const CTTableRecord &rec)
{
    QString  initial_statement = CTQueryParser::xmlStatement(
                CTQueryParser::UpdateStatement, p_table_data->table_name,
                rec);
    /*Getting only the primary keys for the where statement*/
    QList<CTTableField> primaryKeys;
    for(int i = 0; i < p_table_data->constraints.size(); i++)
    {
        if(p_table_data->constraints.at(i).first == "PRIMARY KEY")
            primaryKeys.append(p_table_data->constraints.at(i).second);
    }
    CTTableRecord whereRec = p_table_data->record();
    for(int i =0; i< primaryKeys.size(); i++ )
    {
        whereRec.setValue(primaryKeys.at(i).name(),rec.value(
                              primaryKeys.at(i).name()));
    }
    QString where_statement = CTQueryParser::xmlStatement(
                CTQueryParser::WhereStatement, p_table_data->table_name,
                whereRec);
    execParsedQuery(initial_statement, where_statement);
    return true;
}

bool CTTableModel::deleteRowFromTable(const CTTableRecord &rec)
{
    QString initial_statement = CTQueryParser::xmlStatement(
                CTQueryParser::DeleteStatement, p_table_data->table_name,
                rec);
    /*Getting only the primary keys for the where statement*/
    QList<CTTableField> primaryKeys;
    for(int i = 0; i < p_table_data->constraints.size(); i++)
    {
        if(p_table_data->constraints.at(i).first == "PRIMARY KEY")
            primaryKeys.append(p_table_data->constraints.at(i).second);
    }

    CTTableRecord whereRec = p_table_data->record();
    for(int i =0; i< primaryKeys.size(); i++ )
    {
        whereRec.setValue(primaryKeys.at(i).name(),rec.value(
                              primaryKeys.at(i).name()));
    }
    QString where_statement = CTQueryParser::xmlStatement(
                CTQueryParser::WhereStatement, p_table_data->table_name,
                whereRec);
    execParsedQuery(initial_statement, where_statement);
    return true;
}

QHash<QString,QString> CTTableModel::record(const QModelIndex &index)
{
    if (!index.isValid())
        return QHash<QString,QString>();

    if (index.row() >= rowCount() || index.row() < 0)
        return QHash<QString,QString>();
    return map_to_hash(p_table_data->getRecord(index.row()));
}

void CTTableModel::copyRecord(const QModelIndex &i)
{
    insertRows(0,1,QModelIndex());
    QModelIndex _indexFrom;
    QModelIndex _indexTo;
    CTTableRecord rec = p_table_data->getRecord(i.row());
    for(int column = 1; column < columnCount(); column ++)
    {
        _indexFrom = index(i.row(),column,QModelIndex());
        _indexTo = index(rowCount() - 1, column, QModelIndex());
        if(rec.value("id") != rec.field("id").type())
            rec.setValue("id", rec.field("id").type());
        if(column == 5)
        {
            setData(_indexTo, data(_indexFrom).toString() + "(copy)");
            rec.setValue(column,data(_indexFrom).toString() + "(copy)");
        }
        else
        {
            setData(_indexTo, data(_indexFrom));
            rec.setValue(column,data(_indexFrom).toString());
        }
    }
    insertRowIntoTable(rec);
}


void CTTableModel::deleteRecord(const QModelIndex &index)
{
    removeRows(index.row(),1,QModelIndex());
}

void CTTableModel::save(QHash<QString,QString> record)
{
    qDebug() << Q_FUNC_INFO << record;
    CTTableRecord rec = map_from_hash(record);

    /*Check if new record, case of empty id field*/
    if(rec.value("id") == rec.field("id").type())
    {
        /*Insert a new row in the table*/
        insertRows(0, 1, QModelIndex());
        QModelIndex newRowIndex;
        for(int i = 0 ; i < columnCount() ; i++)
        {
            newRowIndex = index(rowCount() -1, i, QModelIndex());
            if (newRowIndex.isValid())
                setData(newRowIndex, rec.value(i));
            else
                qDebug() << "CTableModel::save invalid index!";
        }
        insertRowIntoTable(rec);
    }
    else
    {
        /*Change a row in the table*/
        int row = getIndex(record["id"]).row();
        QModelIndex _index;
        for(int i = 0 ; i < columnCount() ; i++)
        {
            _index = index(row, i, QModelIndex());
            if(_index.isValid())
                setData(_index, rec.value(i));
            else
                qDebug() << "CTableModel::save invalid index!";
        }
        updateRowInTable(rec);
    }
}


QModelIndex CTTableModel::getIndex(QString id)
{
        QModelIndex _index;
        for(int i = 0; i < rowCount(); i++)
        {
            if(data(index(i,0)).toString() == id)
            {
                _index = index(i,0);
            }
        }
        return _index;
}


/*auxiliary functions for the scenario designer module operates on hash */

QHash<QString,QString> CTTableModel::map_to_hash(const CTTableRecord &rec)
{
    QHash<QString,QString> out;
    for(int i = 0; i< rec.count(); i++)
    {
        out[rec.fieldName(i)] = rec.value(i);
    }
    return out;
}

CTTableRecord CTTableModel::map_from_hash(QHash<QString,QString> &scenario)
{
    CTTableRecord rec = p_table_data->record();
    foreach(QString fieldName, scenario.keys())
    {
        rec.setValue(fieldName, scenario[fieldName]);
    }
    return rec;
}
/*************************************************************/
