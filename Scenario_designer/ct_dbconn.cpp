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

///*To make sure that the table is not empty, since the update might explode*/
//void CTDBConn::checkScenarioTable(){

//    QSqlQuery query;
//    query.prepare("SELECT xml_description FROM test_scenario WHERE id = 1");
//    if(!query.exec()){
//        qDebug() << "CTDBConn::checkScenarioTable() there was an error";
//    }else
//    {
//       if(!query.next()){
//            qDebug() << "CTDBConn::checkScenarioTable() found no entries in the test_scenario table";
//            db.transaction();
//            QSqlQuery query2;
//            query2.prepare("INSERT INTO test_scenario(id, xml_description)"
//                           "VALUES(1,' ')");
//            if(!query2.exec()){
//                qDebug() << "CTDBConn::checkScenarioTable() there was an error";
//                db.rollback();
//            }else
//            {
//                db.commit();
//            }
//       }else
//       {
//          qDebug() << "CTDBConn::checkScenarioTable() found an entry in the test_scenario table";
//       }
//   }
//}


void CTDBConn::insertScenario(QHash<QString,QString> scenario)
{
    db.transaction();
    QSqlQuery query;
    query.prepare("UPDATE test_scenario SET xml_description = :xml_description WHERE id = 1");
    query.bindValue(":xml_description",scenario["xml_description"]);
    if(!query.exec())
    {
        qDebug() << "CTDBConn::insertScenario into test_scenario not successful!" << query.lastError();
        db.rollback();
    }
    else
    {
        qDebug() << "CTDBConn::insertScenario into test_scenario successful!" ;
        db.commit();
    }
}

QSqlDatabase CTDBConn::getConnection(){
    QSqlDatabase db = QSqlDatabase::database();
    return db;
}
