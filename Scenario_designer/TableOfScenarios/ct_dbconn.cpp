#include "ct_dbconn.h"

CTDBConn::CTDBConn(QObject *parent) :
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

QSqlDatabase CTDBConn::getConnection(){
    QSqlDatabase db = QSqlDatabase::database();
    return db;
}
