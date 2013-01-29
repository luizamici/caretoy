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

    if (index.row() >= m_numRows || index.row() < 0)
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
                return tr("Execution date");
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

QVariant CTXMLTableOfScenarios::record(const QModelIndex &index) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_numRows || index.row() < 0)
        return QVariant();
    return m_data.at(index.row());
}
