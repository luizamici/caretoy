#include "queryparser.h"

QueryParser::QueryParser(QObject *parent) :
    QObject(parent)
{
}

QHash<QString, QVariant> QueryParser::parse(const QByteArray &stmt)
{
    qDebug() << stmt;

    QXmlStreamReader reader(stmt);
    while(!reader.isEndElement())
    {
        reader.readNext();
        if(reader.isStartElement() & reader.name() == "select")
            return selectStatement(stmt);
        else if(reader.isStartElement() & reader.name() == "update")
            return updateStatement(stmt);
        else if(reader.isStartElement() & reader.name() == "insert")
            return insertStatement(stmt);
        else if(reader.isStartElement() & reader.name() == "delete")
            return deleteStatement(stmt);
    }
    return QHash<QString,QVariant>();
}


QHash<QString, QVariant> QueryParser::selectStatement(const QByteArray &stmt)
{
    QHash<QString, QVariant> out;
    out["type"] = QVariant("select");
    QString tableName;
    QStringList fieldNames;

    QXmlStreamReader reader(stmt);

    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() & reader.name()== "table")
        {
            tableName = reader.attributes().value("name").toString();
        }
        if(reader.isStartElement() & reader.name()== "field")
            fieldNames.append(reader.attributes().value("name").toString());
    }
    QString s("SELECT ");
    foreach(QString field, fieldNames)
    {
        s += field + ", ";
    }
    s.remove(s.length()-2, 1);
    s += "FROM " + tableName;
    out["statement"] = QVariant(s);
    out["table"] = QVariant(tableName);
    return out;
}

QHash<QString,QVariant>  QueryParser::insertStatement(const QByteArray &stmt)
{
    QHash<QString,QVariant> out;
    out["type"] = QVariant("insert");
    QString tableName;
    QStringList fieldNames;
    QStringList values;
    QXmlStreamReader reader(stmt);

    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() & reader.name()== "table")
        {
            tableName = reader.attributes().value("name").toString();
        }
        if(reader.isStartElement() & reader.name()== "field")
            fieldNames.append(reader.attributes().value("name").toString());
        if(reader.isCharacters() || reader.isCDATA())
        {
            if(!reader.text().toString().trimmed().isEmpty())
                values.append(reader.text().toString());
        }
    }

    QString s("INSERT INTO ");
    s += tableName + "( ";
    foreach(QString field, fieldNames)
    {
        s += field + ", ";
    }
    s.remove(s.length()-2, 1); //remove last ","
    s += ") VALUES( ";
    foreach(QString field, fieldNames)
    {
        s += "? , ";
    }
    s.remove(s.length()-2, 1); //remove last ","
    s+= ")";
    qDebug() << s;
    out["statement"] = QVariant(s);
    qDebug() << values;

    out["values"] = QVariant(values);
    return out;
}

QHash<QString,QVariant> QueryParser::updateStatement(const QByteArray &stmt)
{
    bool isCondition = false;
    QHash<QString,QVariant> out;
    out["type"] = QVariant("update");
    QString tableName;
    QStringList fieldNames;
    QStringList values;

    QString condition;
    QXmlStreamReader reader(stmt);

    QString s("UPDATE ");
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() & reader.name()== "table")
        {
            tableName = reader.attributes().value("name").toString();
        }
        if(reader.isStartElement() & reader.name()== "field" & !isCondition)
            fieldNames.append(reader.attributes().value("name").toString());
        if(reader.isCharacters() || reader.isCDATA())
        {
            if(!reader.text().toString().trimmed().isEmpty())
                values.append(reader.text().toString());
        }
        if(reader.isStartElement() && reader.name() == "where")
        {
            isCondition = true;
            s += tableName + " SET ";
            foreach(QString field, fieldNames)
            {
                s += field + " = ? ,";
            }
            s.remove(s.length()-1, 1); //remove last ","
            s += " WHERE ";
        }

        if(reader.isStartElement() && reader.name()=="field" && isCondition)
        {
            condition += reader.attributes().value("name") ;
        }
        if(reader.isStartElement() && reader.name() == "op")
        {
            if(reader.attributes().value("type") == "eq")
                condition += " = ? AND ";
        }
        if(reader.isEndElement() && reader.name() == "where")
        {
            condition.remove(condition.length() -4, 3); //remove last 'AND'
            s += condition;
        }
    }
    out["statement"] = QVariant(s);
    out["values"] = QVariant(values);
    return out;
}

QHash<QString,QVariant> QueryParser::deleteStatement(const QByteArray &stmt)
{
    QHash<QString,QVariant> out;
    out["type"] = QVariant("delete");
    QString tableName;
    QStringList values;
    QString condition;

    QString s("DELETE FROM ");
    QXmlStreamReader reader(stmt);
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() & reader.name()== "table")
        {
            tableName = reader.attributes().value("name").toString();
        }
        if(reader.isCharacters() || reader.isCDATA())
        {
            if(!reader.text().toString().trimmed().isEmpty())
                values.append(reader.text().toString());
        }
        if(reader.isStartElement() && reader.name() == "where")
        {
            s += tableName + " WHERE ";
        }
        if(reader.isStartElement() && reader.name() == "field")
        {
            condition += reader.attributes().value("name");
        }
        if(reader.isStartElement() && reader.name() == "op")
        {
            if(reader.attributes().value("type") == "eq")
                condition += " = ? AND ";
        }
        if(reader.isEndElement() && reader.name() == "where")
        {
            condition.remove(condition.length() -4, 3); //remove last 'AND'
            s += condition;
        }
    }
    out["statement"] = QVariant(s);
    out["values"] = QVariant(values);
    return out;
}

QHash<QString, QVariant> QueryParser::parseForAuthentication(QString username,
                                                    QString psswd)
{
    QHash<QString,QVariant> out;
    //"SELECT id FROM users WHERE nickname = :username AND password = digest(:password, 'sha512')"
    out["statement"] = "SELECT id FROM test_users WHERE nickname = ? AND password = ? ";
    QStringList values;
    values << username << psswd;
    out["values"] = QVariant(values);
    out["table"] = "test_users";
    return out;
}