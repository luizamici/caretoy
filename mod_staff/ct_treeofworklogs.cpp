#include "ct_treeofworklogs.h"

CTTreeOfWorkLogs::CTTreeOfWorkLogs(QWidget *parent) :
    QTreeView(parent)
{
}


void CTTreeOfWorkLogs::init(QByteArray table_data)
{
    xmlTree = new CTTreeModel(table_data,this->parentWidget());
    this->setModel(xmlTree);
    this->setColumnHidden(1, true);
    this->expandAll();
}


QString CTTreeOfWorkLogs::getWorklog(QModelIndex index)
{
    qDebug() << "retreiveing from :" << index.row() << index.column();
    return xmlTree->getChildAt(index);
}
