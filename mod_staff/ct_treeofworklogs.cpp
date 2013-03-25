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


QStringList CTTreeOfWorkLogs::getWorklog(QModelIndex index)
{
    qDebug() << "retreiveing from :" << index.row() << index.column();
    return xmlTree->getChildAt(index);
}

QStringList CTTreeOfWorkLogs::save(QString log, QString id)
{
    return xmlTree->updateLog(log, id);
}

QStringList CTTreeOfWorkLogs::saveNew(QString log)
{
    return xmlTree->saveLog(log);
}
