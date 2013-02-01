#include "ct_tablemodel.h"
#include "ct_queryparser.h"


CTTableModel::CTTableModel(CTTableData* table_data, QObject *parent)
    : QAbstractTableModel(parent)
{
    p_table_data = table_data;
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

    return p_table_data->data.at(index.row()).at(index.column());
}


QVariant CTTableModel::headerData(
        int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if(section < p_table_data->field_names.size())
            return p_table_data->field_names.at(section).first;
    }
    else
    {
        return QVariant();
    }
}

bool CTTableModel::insertRows(int position, int rows,
                                       const QModelIndex &index)
{
    Q_UNUSED(position);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        QStringList record;
        for(int column = 0; column < columnCount(); column++)
            record.append(" ");
        p_table_data->data.append(record);
        p_table_data->numRows++;
    }
    endInsertRows();
    return true;
}

bool CTTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        p_table_data->data.removeAt(position);
        p_table_data->numRows--;
    }

    endRemoveRows();
    return true;
}

bool CTTableModel::setData(const QModelIndex &index,
                                    const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();
        QStringList record = p_table_data->data.at(row);
        record.replace(index.column(),value.toString()) ;
        p_table_data->data.replace(row,record);

        emit(dataChanged(index, index));
        return true;
    }
    return false;
}

bool CTTableModel::insertRowIntoTable(const QHash<QString,QString> &values)
{
    CTQueryParser queryParser;
    QString initial_statement = queryParser.statement(
                CTQueryParser::InsertStatement, p_table_data->table_name,
                values);
    CTXmlDataParser::execParsedQuery(initial_statement);
    return true;
}

QHash<QString, QString> CTTableModel::record(const QModelIndex &index)
{
    //TODO implement this using data()
    if (!index.isValid())
        return QHash<QString,QString>();

    if (index.row() >= rowCount() || index.row() < 0)
        return QHash<QString,QString>();
    return mapToHash(p_table_data->data.at(index.row()));
}

void CTTableModel::copyRecord(const QModelIndex &i)
{
    insertRows(0,1,QModelIndex());
    QModelIndex _indexFrom;
    QModelIndex _indexTo;
    for(int column = 1; column < columnCount(); column ++)
    {
        _indexFrom = index(i.row(),column,QModelIndex());
        _indexTo = index(rowCount() - 1, column, QModelIndex());
        if(column == 5)
            setData(_indexTo, data(_indexFrom).toString() + "(copy)");
        else
            setData(_indexTo, data(_indexFrom));
    }
}


void CTTableModel::deleteRecord(const QModelIndex &index)
{
    removeRows(index.row(),1,QModelIndex());
}

void CTTableModel::save(QHash<QString, QString> record_hash)
{
    /*Mapping from hash to string list*/
    QStringList record = map(record_hash);

    /*Check if new record*/
    if(record_hash["id"] == "")
    {
        /*Insert a new row in the table*/
        insertRows(0, 1, QModelIndex());
        QModelIndex newRowIndex;
        for(int i = 0 ; i < columnCount() ; i++)
        {
            newRowIndex = index(rowCount() -1, i, QModelIndex());
            if (newRowIndex.isValid())
                setData(newRowIndex, record.at(i));
            else
                qDebug() << "CTableModel::save invalid index!";
        }
        //Prepare INSERT query for the db in xml
        insertRowIntoTable(this->record(newRowIndex));
    }
    else
    {
        /*Change a row in the table*/
        int row = getIndex(record_hash["id"]).row();
        QModelIndex _index;
        for(int i = 0 ; i < columnCount() ; i++)
        {
            _index = index(row, i, QModelIndex());
            if(_index.isValid())
                setData(_index, record.at(i));
            else
                qDebug() << "CTableModel::save invalid index!";
        }
        //Prepare UPDATE query for the db in xml
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


QStringList CTTableModel::map(QHash<QString, QString> record_hash)
{
    QStringList record;
    for(int i = 0; i< record_hash.size(); i++)
    {
        record.append(record_hash[p_table_data->field_names.at(i).first]);
    }
    return record;
}

QHash<QString,QString> CTTableModel::mapToHash(QStringList record)
{
    QHash<QString,QString> record_hash;
    for(int i = 0; i < record.size(); i++)
    {
        record_hash[p_table_data->field_names.at(i).first] = record.at(i);
    }
    return record_hash;
}
