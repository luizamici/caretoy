#ifndef CT_XMLTABLEOFSCENARIOS_H
#define CT_XMLTABLEOFSCENARIOS_H

#include <QWidget>
#include <QDebug>
#include <QAbstractTableModel>

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

    QVariant record(const QModelIndex &index) const;
    
private:
    int m_numRows;
    int m_numColumns;
    QList<QStringList> m_data;

signals:
    
public slots:
    
};

#endif // CT_XMLTABLEOFSCENARIOS_H
