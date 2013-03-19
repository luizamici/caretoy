#include "ct_dbconnadmin.h"
#include <QDebug>


CTDBConnAdmin::CTDBConnAdmin(QObject *parent) :
    QObject(parent)
{
    conn = new CTDBConn();
    conn->initializeDB("caretoy","c@reTOypROJECT");
}

QSqlDatabase CTDBConnAdmin::getDBConnection(){
    return(conn->getConnection());
}

void CTDBConnAdmin::closeDBConnection(){
    conn->closeConnection();
}

void CTDBConnAdmin::authenticate(const QString &nickname, const QString &password){
    /*Returned values from the DB,further to be added the last connection info
     *usercredentials["id_staff"],
     *usercredentials["name"],
     *usercredentials["lastname"]
     */
    userCredentials = conn->authenticate(nickname,password);
    if(userCredentials.isEmpty()){emit authorizationFailed();}
    else{emit userAuthorized(userCredentials);}
}

void CTDBConnAdmin::saveNewLog(QHash<QString, QString> newLogToSave){
    /*
     *Request for saving a new work log of the logged-in user to the DB
     *worklogToSave["relativetimestamp"],
     *worklogToSave["log"]
     */
    if(conn->saveNewWorkLog(userCredentials["id_staff"],newLogToSave)){emit refreshStaffModule("insertednewworklog");}
}

void CTDBConnAdmin::updateLog(QHash<QString, QString> worklogToUpdate){
    /*
     *Request for updating an existing work log of the logged-in user to the DB
     *worklogToUpdate["id_worklog"],
     *worklogToUpdate["relativetimestamp"],
     *worklogToUpdate["log"]
     */
    if(conn->updateWorklog(worklogToUpdate)){emit refreshStaffModule("updatedworklog");}
}

QMap<QPair<QString, QString>, QString> CTDBConnAdmin::getWorkLogs(QString id_user){

    QMap<QPair<QString,QString>,QString> worklogs;
    worklogs = conn->getWorkLogs(id_user);
    return worklogs;
}

void CTDBConnAdmin::insertUser(QSqlRecord &rec){

    QHash<QString,QString> newUser;
    newUser["id"] = rec.value("id").toString();
    QString rand = gen_random(5);
    newUser["nickname"] = rand;
    newUser["password"] = rand;
    conn->insertUser(newUser);
}

QStringList CTDBConnAdmin::getScenariosForPatient(QString id_patient){
    return(conn->getScenariosForPatient(id_patient));
}

QString CTDBConnAdmin::gen_random(const int len) {
    QChar s[len];
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[len] = 0;
    QString stringa(s);
    return stringa;
}
