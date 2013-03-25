#ifndef CT_TREEMODEL_H
#define CT_TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QXmlStreamReader>

class CTTreeItem;


class CTTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    CTTreeModel(const QByteArray &data, QObject *parent = 0);
    ~CTTreeModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QStringList getChildAt(const QModelIndex &parent);
    QStringList updateLog(QString log, QString id);
    QStringList saveLog(QString log);

private:
    void setupModelData(const QStringList &parents, const QList<QStringList> &children, CTTreeItem *parent);

    QStringList getParents(QByteArray data);
    QList<QStringList> getChildren(QByteArray data);
    QString formatDate(QString date);

    CTTreeItem *rootItem;
};


#endif
