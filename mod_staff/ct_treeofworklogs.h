#ifndef CT_TREEOFWORKLOGS_H
#define CT_TREEOFWORKLOGS_H

#include <QWidget>
#include <QTreeView>
#include <QAbstractItemView>
#include <QSortFilterProxyModel>
#include "DbTableXML/ct_tabledata.h"
#include "DbTableXML/ct_tablemodel.h"
#include "DbTreeXml/ct_treemodel.h"

class CTTreeOfWorkLogs : public QTreeView
{
    Q_OBJECT
public:
    explicit CTTreeOfWorkLogs(QWidget *parent = 0);

    void init(QByteArray table_data);

    QStringList getWorklog(QModelIndex index);
    QStringList getWorklogByDate(QDate date);
    QStringList *getParentNodes();
    QStringList save(QString log, QString id);
    QStringList saveNew(QString log);
    QStringList deleteLog(QString id);
    CTTreeModel *xmlTree;

private:
    QSortFilterProxyModel *filterModel; 

    
public slots:
    
};

#endif // CT_TREEOFLOGS_H
