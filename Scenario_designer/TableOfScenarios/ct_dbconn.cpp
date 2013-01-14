#include "ct_dbconn.h"

CTDBConn::CTDBConn(QObject *parent) :
    QObject(parent)
{
    Log4Qt::Logger *p_logger = Log4Qt::Logger::logger(QLatin1String("CTDBConn"));
    p_logger->info("Connection to the DB being attempted...");

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(55555);
    db.setDatabaseName("ljubljana_test");
    db.setUserName("caretoy");
    db.setPassword("c@reTOypROJECT");
    if(!db.open()){
      qDebug() << "Connection not successful!" <<  db.lastError().text() ;
      p_logger->warn("Unable to establish a connection to the DB ljubljana_test",db.lastError().text());

    }
    else{
        qDebug() << "Connection to DB ljubljana_test opened";
        p_logger->info("Connection to the DB established successfully!");
    }
}

QSqlDatabase CTDBConn::getConnection(){
    QSqlDatabase db = QSqlDatabase::database();
    return db;
}
