#ifndef CT_TREEOFWORKLOGS_H
#define CT_TREEOFWORKLOGS_H

#include <QWidget>
#include <QTreeView>
#include "DbTableXML/ct_tabledata.h"
#include "DbTableXML/ct_tablemodel.h"
#include <QAbstractItemView>

#include "DbTreeXml/ct_treemodel.h"

class CTTreeOfWorkLogs : public QTreeView
{
    Q_OBJECT
public:
    explicit CTTreeOfWorkLogs(QWidget *parent = 0);

    void init(QByteArray table_data);

    QString getWorklog(QModelIndex index);
    CTTreeModel *xmlTree;
    
signals:
    
public slots:
    
};

#endif // CT_TREEOFLOGS_H
