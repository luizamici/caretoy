#include "ct_treeofworklogs.h"

CTTreeOfWorkLogs::CTTreeOfWorkLogs(QWidget *parent) :
    QTreeView(parent)
{
    this->setSortingEnabled(true);

}


void CTTreeOfWorkLogs::init(QByteArray table_data)
{
    xmlTree = new CTTreeModel(table_data,this->parentWidget());
    filterModel = new QSortFilterProxyModel();
    filterModel->setSourceModel(xmlTree);
    this->setModel(filterModel);
    this->setColumnHidden(1, true);
    this->expandAll();
}


QStringList CTTreeOfWorkLogs::getWorklog(QModelIndex index)
{
    qDebug() << "retreiveing from :" << index.row() << index.column();
    return xmlTree->getChildAt(filterModel->mapToSource(index));
}

QStringList CTTreeOfWorkLogs::getWorklogByDate(QDate date)
{
    QStringList _worklog;
    filterModel->setFilterKeyColumn(0);
    filterModel->setFilterRegExp(date.toString("yyyy-MM-dd"));
    QModelIndex index = filterModel->mapToSource(filterModel->index(0,0));
    if(index.isValid())
        _worklog = xmlTree->getChildAt(index);
    qDebug() << _worklog;
    filterModel->setFilterRegExp("");
    return _worklog;
}

QStringList* CTTreeOfWorkLogs::getParentNodes()
{
    return xmlTree->parentNodes;
}

QStringList CTTreeOfWorkLogs::save(QString log, QString id)
{
    return xmlTree->updateLog(log, id);
}

QStringList CTTreeOfWorkLogs::saveNew(QString log)
{
    return xmlTree->saveLog(log);
}

QStringList CTTreeOfWorkLogs::deleteLog(QString id)
{
   return xmlTree->deleteLogById(id);
}

