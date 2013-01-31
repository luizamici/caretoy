#ifndef CT_XMLTABLEOFSCENARIOS_H
#define CT_XMLTABLEOFSCENARIOS_H

#include <QWidget>
#include <QDebug>
#include <QAbstractTableModel>

#include "ct_queryparser.h"
#include "ct_xmlparser.h"

class CTXMLTableOfScenarios : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CTXMLTableOfScenarios(int numRows, int numColumns,
                                   QList<QStringList> data,
                                   QObject *parent = 0);

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

    QVariant record(const QModelIndex &index) const;
    void copyRecord(const QModelIndex &i);

    void deleteRecord(const QModelIndex &index);
    void save(QHash<QString, QString> scenario);
    
private:
    int m_numRows;
    int m_numColumns;
    QList<QStringList> m_data;

    bool insertRowIntoTable(const QVariant &values);

    QStringList map(QHash<QString,QString> scenario);
    QHash<QString,QString> mapToHash(QStringList scenario);
    QModelIndex getIndex(QString id_scenario);

signals:
    
public slots:
    
};

#endif // CT_XMLTABLEOFSCENARIOS_H
