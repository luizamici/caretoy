#include "ct_xmltableofscenarios.h"

CTXMLTableOfScenarios::CTXMLTableOfScenarios(int numRows, int numColumns,
                                QList<QStringList> data, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_numRows = numRows;
    m_numColumns = numColumns;
    m_data = data;
}

int CTXMLTableOfScenarios::rowCount(
        const QModelIndex& parent = QModelIndex()) const
{
    return m_numRows;
}

int CTXMLTableOfScenarios::columnCount(
        const QModelIndex& parent = QModelIndex()) const
{
    return m_numColumns;
}

QVariant CTXMLTableOfScenarios::data(
        const QModelIndex& index, int role = Qt::DisplayRole) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    if (index.row() >= rowCount() || index.row() < 0)
        return QVariant();

    return m_data.at(index.row()).at(index.column());
}


QVariant CTXMLTableOfScenarios::headerData(
        int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Id");
            case 1:
                return tr("Execution day");
            case 2:
                return tr("Execution order");
            case 3:
                return tr("Creation date");
            case 4:
                return tr("Last edited");
            case 5:
                return tr("Description");
            case 6:
                return tr("XML");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool CTXMLTableOfScenarios::insertRows(int position, int rows,
                                       const QModelIndex &index)
{
    Q_UNUSED(position);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        QStringList record;
        for(int column = 0; column < columnCount(); column++)
            record.append(" ");
        m_data.append(record);
        m_numRows++;
    }
    endInsertRows();
    return true;
}

bool CTXMLTableOfScenarios::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        m_data.removeAt(position);
        m_numRows--;
    }

    endRemoveRows();
    return true;
}

bool CTXMLTableOfScenarios::setData(const QModelIndex &index,
                                    const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();
        QStringList record = m_data.at(row);
        record.replace(index.column(),value.toString()) ;
        m_data.replace(row,record);

        emit(dataChanged(index, index));
        return true;
    }
    return false;
}

QVariant CTXMLTableOfScenarios::record(const QModelIndex &index) const
{
    //TODO implement this using data()
    if (!index.isValid())
        return QVariant();

    if (index.row() >= rowCount() || index.row() < 0)
        return QVariant();
    return m_data.at(index.row());
}

void CTXMLTableOfScenarios::copyRecord(const QModelIndex &i)
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


void CTXMLTableOfScenarios::deleteRecord(const QModelIndex &index)
{
    removeRows(index.row(),1,QModelIndex());
}

void CTXMLTableOfScenarios::save(QHash<QString, QString> scenario)
{
    /*Mapping from hash to string list*/
    QStringList record = map(scenario);

    /*Check if new scenario*/
    if(scenario["id"] == "")
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
                qDebug() << "CTXMLTableOfScenarios::save invalid index!";
        }
        //Prepare INSERT query for the db in xml
    }
    else
    {
        /*Change a row in the table*/
        int row = getIndex(scenario["id"]).row();
        QModelIndex _index;
        for(int i = 0 ; i < columnCount() ; i++)
        {
            _index = index(row, i, QModelIndex());
            if(_index.isValid())
                setData(_index, record.at(i));
            else
                qDebug() << "CTXMLTableOfScenarios::save invalid index!";
        }
        //Prepare UPDATE query for the db in xml
    }
}


QModelIndex CTXMLTableOfScenarios::getIndex(QString id_scenario)
{
        QModelIndex _index;
        for(int i = 0; i < rowCount(); i++)
        {
            if(data(index(i,0)).toString() == id_scenario)
            {
                _index = index(i,0);
            }
        }
        return _index;
}


QStringList CTXMLTableOfScenarios::map(QHash<QString, QString> scenario)
{
    QStringList record;
    record.append(scenario["id"]);
    record.append(scenario["execution_day"]);
    record.append(scenario["execution_order"]);
    record.append(scenario["creation_date"]);
    record.append(scenario["last_edited"]);
    record.append(scenario["description"]);
    record.append(scenario["xml_description"]);
    return record;
}
