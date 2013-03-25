#include <QtGui>
#include "ct_treemodel.h"

#include "DbTableXML/ct_tablefield.h"
#include "DbTableXML/ct_tablerecord.h"
#include "DbTableXML/ct_queryparser.h"

class CTTreeItem
{
public:
    CTTreeItem(const QList<QVariant> &data, CTTreeItem *parent = 0);
    ~CTTreeItem();

    void appendChild(CTTreeItem *child);

    CTTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    CTTreeItem *parent();

    QList<CTTreeItem*> childItems;
    QList<QVariant> itemData;
    CTTreeItem *parentItem;
};


CTTreeItem::CTTreeItem(const QList<QVariant> &data, CTTreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}


CTTreeItem::~CTTreeItem(){qDeleteAll(childItems);}

void CTTreeItem::appendChild(CTTreeItem *item){childItems.append(item);}

CTTreeItem *CTTreeItem::child(int row){return childItems.value(row);}

int CTTreeItem::childCount() const{return childItems.count();}

int CTTreeItem::columnCount() const{return itemData.count();}

QVariant CTTreeItem::data(int column) const{return itemData.value(column);}

CTTreeItem *CTTreeItem::parent(){return parentItem;}

int CTTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CTTreeItem*>(this));

    return 0;
}

/***********************************************************************/


CTTreeModel::CTTreeModel(const QByteArray &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Timestamps" << "Id" ;
    rootItem = new CTTreeItem(rootData);
    setupModelData(getParents(data), getChildren(data),  rootItem);
}

CTTreeModel::~CTTreeModel()
{
    delete rootItem;
}


QStringList CTTreeModel::getChildAt(const QModelIndex &parent)
{
    QStringList child;
    CTTreeItem *item = static_cast<CTTreeItem*>(parent.internalPointer());
    if(item->childCount() > 0)
    {
        child << item->child(0)->data(0).toString();
        child << item->child(0)->data(1).toString();
    }
    else
    {
        child << item->data(0).toString();
        child << item->data(1).toString();
    }
    return child;
}

int CTTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<CTTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}


QVariant CTTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    CTTreeItem *item = static_cast<CTTreeItem*>(index.internalPointer());

    return item->data(index.column());
}


Qt::ItemFlags CTTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant CTTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}


QModelIndex CTTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    CTTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<CTTreeItem*>(parent.internalPointer());

    CTTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


QModelIndex CTTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CTTreeItem *childItem = static_cast<CTTreeItem*>(index.internalPointer());
    CTTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


int CTTreeModel::rowCount(const QModelIndex &parent) const
{
    CTTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<CTTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}


void CTTreeModel::setupModelData(const QStringList &parents, const QList<QStringList> &children, CTTreeItem *parent)
{
    QList<CTTreeItem*> _parents;
    _parents << parent;

    int numberLogs = 0;
    foreach(QString item,parents)
    {
        QList<QVariant> data;
        data << item <<QString(" ");
        CTTreeItem *newParent = new CTTreeItem(data, _parents.last());
        _parents.last()->appendChild(newParent); //new child in the rootItem
        _parents << _parents.last()->child(_parents.last()->childCount()-1);
        data.clear();
        data << children.at(numberLogs).at(0) << children.at(numberLogs).at(1);
        _parents.last()->appendChild(new CTTreeItem(data, _parents.last()));
        if(!_parents.isEmpty())
        {
            _parents.pop_back();
        }
        numberLogs++;
    }
}

//The secondary leaves of the tree(after the root leaf) are the
//timestamps of the logs
QStringList CTTreeModel::getParents(QByteArray data)
{
    QStringList listParents;
    bool write;
    QXmlStreamReader reader(data);
    while(!reader.atEnd())
    {
        reader.readNext();

        if(reader.isStartElement() &&  reader.name().toString() == "relativetimestamp")
            write = true;
        if(reader.isEndElement() && reader.name().toString() == "relativetimestamp")
            write = false;

        /*When a new block is found*/
        if(reader.isCharacters() && !reader.text().toString().trimmed().isEmpty() && write == true)
        {
            listParents << formatDate(reader.text().toString());
        }
    }
    return listParents;
}

QList<QStringList> CTTreeModel::getChildren(QByteArray data)
{
    QList<QStringList> _listChildren;
    QStringList child;

    QXmlStreamReader reader(data);
    while(!reader.atEnd())
    {
        reader.readNext();

        if(reader.isStartElement() &&  reader.name().toString() == "log")
        {
            child = QStringList();
        }
        if(reader.isCharacters() && !reader.text().toString().trimmed().isEmpty())
        {
            child.append(reader.text().toString());
        }
        if(reader.isEndElement() && reader.name() == "row")
        {
            _listChildren << child;
        }
    }
    return _listChildren;
}


//Removes the 'T' character from the DB returned timestamp
QString CTTreeModel::formatDate(QString date)
{
    return date.replace("T", " ");
}

QStringList CTTreeModel::updateLog(QString log, QString id)
{
    QStringList stmt;
    CTTableField field;
    field.setName("log");
    field.setValue(log);
    CTTableRecord rec;
    rec.append(field);
    QString  initial_statement = CTQueryParser::xmlStatement(
                CTQueryParser::UpdateStatement, "worklogs",
                rec);

    CTTableRecord whereRec;
    field.clear();
    field.setName("id");
    field.setValue(id);
    whereRec.append(field);

    QString where_statement = CTQueryParser::xmlStatement(
                CTQueryParser::WhereStatement, "worklogs",
                whereRec);

    stmt << initial_statement << where_statement;
    return stmt;
}

QStringList CTTreeModel::saveLog(QString log)
{
    QStringList stmt;
    CTTableRecord rec;
    CTTableField field;
    field.setName("log");
    field.setValue(log);
    rec.append(field);
    field.clear();
    field.setName("relativetimestamp");
    field.setValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
    rec.append(field);
    field.clear();
    field.setName("staff");
    field.setValue(qApp->property("UserID").toString());
    rec.append(field);

    QString  initial_statement = CTQueryParser::xmlStatement(
                CTQueryParser::InsertStatement, "worklogs",
                rec);

    QString where_statement = "";
    stmt << initial_statement << where_statement;
    return stmt;
}
