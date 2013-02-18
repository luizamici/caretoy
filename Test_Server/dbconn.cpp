#include "dbconn.h"

DBConn::DBConn(QObject *parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(55555);
    db.setDatabaseName("ljubljana_test");
    db.setUserName("caretoy");
    db.setPassword("c@reTOypROJECT");
    if(!db.open()){
      qDebug() << "Connection not successful!" <<  db.lastError().text() ;
    }
    else{
        qDebug() << "Connection to DB ljubljana_test opened";
    }
}

/*
 *Executes query passed as statement to prepare and values to bind
 *In case of select table, the result is serialized in xml and
 *written in the socket
 */
bool DBConn::exec(QHash<QString, QVariant> query)
{
    qDebug() << query["type"].toString();
    qDebug() << query["statement"].toString();
    QList<QSqlRecord> table_info;
    /*
     *Getting tables's info in case of select table
     */
    if(query["type"].toString() == "select")
    {
        QSqlQuery sqlQuery;
        sqlQuery.prepare("SELECT ordinal_position, tab_columns.column_name, data_type, "
                      "column_default, character_maximum_length,numeric_precision, "
                      "is_nullable,tab_constraints.constraint_type, "
                      "col_constraints.constraint_name, col_check_constraints.check_clause "
                      "FROM information_schema.columns AS tab_columns "
                      "LEFT OUTER JOIN "
                      "information_schema.constraint_column_usage AS col_constraints"
                      "	ON tab_columns.table_name = col_constraints.table_name AND "
                      "	tab_columns.column_name = col_constraints.column_name "
                      "	LEFT OUTER JOIN "
                      "	information_schema.table_constraints AS tab_constraints "
                      "	ON tab_constraints.constraint_name = col_constraints.constraint_name "
                      "	LEFT OUTER JOIN "
                      "	information_schema.check_constraints AS col_check_constraints "
                      "	ON col_check_constraints.constraint_name = tab_constraints.constraint_name "
                      "	WHERE tab_columns.table_name = :table_name AND "
                      "	tab_columns.table_schema = :table_schema "
                      "	ORDER BY ordinal_position;");
        sqlQuery.bindValue(":table_name",query["table"]);
        sqlQuery.bindValue(":table_schema", "public");
        if(sqlQuery.exec())
        {
            while(sqlQuery.next())
            {
                table_info.append(sqlQuery.record());
            }
        }
    }

    QSqlQuery sqlQuery;
    sqlQuery.prepare(query["statement"].toString());

    if(!query["values"].toStringList().isEmpty())
    {
        foreach(QString value, query["values"].toStringList())
        sqlQuery.addBindValue(value);
    }

    if(sqlQuery.exec())
    {
        qDebug() << "OK!";
        QList<QSqlRecord> recs;
        while(sqlQuery.next())
        {
            recs.append(sqlQuery.record());
        }
        if(!recs.isEmpty())
            requestToWrite(DataToXml::tableToXml(
                               query["table"].toString(),table_info,recs));
    }
    else
        qDebug() << sqlQuery.lastError().text();
    return true;
}
