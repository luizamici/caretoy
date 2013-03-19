#include <QSqlDatabase>
#include "ct_dbconn.h"
#include "ct_logindialog.h"
#include "mod_staff/ct_staffmodule.h"
#include <QtGui>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>


CTDBConn::CTDBConn(QObject *parent) :
   QObject(parent)
{
}

bool CTDBConn::initializeDB(QString username, QString password){
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(55555);
    db.setDatabaseName("ljubljana_test");
    db.setUserName(username);
    db.setPassword(password);
    if(! db.open()){
      qDebug() << "Connection not successful!" <<  db.lastError().text() ;
      return false;
     }
     qDebug() << "Connection to DB CareToy opened";
     return true;

}

QHash<QString, QString> CTDBConn::authenticate(const QString &nickname, const QString &password){

    QHash<QString, QString> userCredentials;
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE nickname = :username AND password = digest(:password, 'sha512')");
    query.bindValue(":username", nickname);
    query.bindValue(":password", password);
    query.exec();
    if(!query.next()){
        qDebug() << "CTDBConn::authenticate() not successful";
    }else{
        QSqlQuery query2;
        query2.prepare("SELECT * FROM staff WHERE id = :id");
        query2.bindValue(":id", query.value(0).toString());
        query2.exec();
        if(!query2.next()){
            qDebug() << "User not found in the staff table";
        }else{
            qDebug() << "Authentication successful for" << query2.value(1).toString();
            userCredentials["id_staff"] = query2.value(0).toString();
            userCredentials["name"] = query2.value(1).toString();
            userCredentials["lastname"] = query2.value(2).toString();
         }
    }
    return userCredentials;
}

bool CTDBConn::saveNewWorkLog(QString id_user, QHash<QString, QString> worklogToSave){

    db.transaction();
    QSqlQuery query;
    query.prepare("INSERT INTO worklogs(log,relativetimestamp)"
                  "VALUES(:worklog,:timestamp) RETURNING id");
    query.bindValue(":timestamp",worklogToSave["relativetimestamp"]);
    query.bindValue(":worklog",worklogToSave["log"]);
    query.exec();
    if(!query.next()){
        qDebug() << "CTDBConn::saveNewWorkLog into 'worklog' not successful!" << query.lastError();

    }else{
        QString id_worklog = query.value(0).toString();
        QSqlQuery query2;
        query2.prepare("INSERT INTO rtherapist_worklog(id_therapist, id_worklog)"
                       "VALUES(:id_therapist,:id_worklog)");
        query2.bindValue(":id_therapist",id_user);
        query2.bindValue(":id_worklog",id_worklog);
        if(!query2.exec()){
            qDebug() << "CTDBConn::saveNewWorkLog into rtherapist_worklog not successful!" << query.lastError();
            db.rollback();
            return false;}
        else{
            db.commit();
            return true;
        }
    }
    return false;
}


bool CTDBConn::updateWorklog(QHash<QString,QString> worklogToUpdate){

    QSqlQuery query;
    query.prepare("UPDATE worklogs SET log = :logtoSave WHERE id = :id_worklog");
    query.bindValue(":logtoSave",worklogToUpdate["log"]);
    query.bindValue(":id_worklog", worklogToUpdate["id_worklog"]);
    if(query.exec()){return true;qDebug() << "OK";}
    else{return false;}
}

QMap<QPair<QString,QString>,QString> CTDBConn::getWorkLogs(QString id_user){

    QMap<QPair<QString,QString>,QString> worklogs;
    QSqlQuery query;
    query.prepare("SELECT id_worklog FROM rtherapist_worklog WHERE id_therapist = :id_therapist");
    query.bindValue(":id_therapist",id_user);
    if(query.exec()){
        for(int i = 0; i < query.size(); i++){
            if(query.next()){
                QString id_worklog = query.value(0).toString();
                QSqlQuery query2;
                query2.prepare("SELECT * FROM worklogs WHERE id = :id_worklog");
                query2.bindValue(":id_worklog",id_worklog);
                query2.exec();
                if(!query2.next()){qDebug() << "CTDBConn::getWorkLogs failed to get log from worklogs table for id_worklog : " << id_worklog;}
                else{
                    /* worklogs[id_worklog,timestamp] = log */
                    QString id_worklog = query2.value(0).toString();
                    QString timestamp = query2.value(2).toString();
                    QString worklog  = query2.value(1).toString();
                    worklogs[QPair<QString,QString>(id_worklog,timestamp)] = worklog;
                }
            }
        }
    }
    return worklogs;
}



void CTDBConn::insertUser(QHash<QString,QString> newUser){
    QSqlQuery query;
    query.prepare("INSERT INTO users(id,nickname, password)"
                  "VALUES(:id,:nickname,digest(:password, 'sha512'))");
    query.bindValue(":id",newUser["id"]);
    query.bindValue(":nickname",newUser["nickname"]);
    query.bindValue(":password",newUser["password"]);
    if(query.exec()){qDebug() << "Insert new user successful";}
    else{qDebug() << "There was an error while inserting new user!";}
}


QStringList CTDBConn::getScenariosForPatient(QString id_patient)
{
     QStringList scenarios;
     QSqlQuery query;
     query.prepare("SELECT id_scenario FROM rpatients_scenarios WHERE id_patient= :id_patient ");
     query.bindValue("id_patient", id_patient);
     if(query.exec())
     {
         for(int i = 0; i < query.size(); i++){
             if(query.next()){
                 scenarios.append(query.value(0).toString());
            }else{qDebug() << "No existing scenarios found for the patient";}
         }
     }else{qDebug() << "CTDBConn::getScenariosForPatient, there was an error with the query";}
     return scenarios;
}





















void CTDBConn::insertQ(const QStringList &stringList, const QString &addworklog){
    QSqlQuery query3;
    query3.prepare("UPDATE staff SET worklog = :addworklog || worklog WHERE id = :id_staff");
    query3.bindValue(":id_staff", stringList.at(0));
    query3.bindValue(":addworklog", addworklog);
    if(!query3.exec()){
        qDebug() << "Operation not succesfull" << query3.lastError();
    }else{
        qDebug() << "Insertion succesful of: " << addworklog;}
}


QString CTDBConn::getWorkLog(QStringList sessionData){

    QSqlQuery query4;
    query4.prepare("SELECT worklog FROM staff WHERE id = :id_user");
    query4.bindValue(":id_user", sessionData.at(0));
    query4.exec();
    while(query4.next()){ return query4.value(0).toString();}
}

//bool CTDBConn::saveWorkLog(QString logtoSave, QStringList sessionData){

//    QSqlQuery query5;
//    query5.prepare("UPDATE staff SET worklog = :logtoSave WHERE id_staff = :id_staff");
//    query5.bindValue(":id_staff", sessionData.at(0));
//    query5.bindValue(":logtoSave", logtoSave);
//    if(!query5.exec()){
//        qDebug()<< "Operation not succesfull" << query5.lastError();
//        return false;
//    }else{qDebug() << "Update of WorkLog succesfull! "; return true;}

//}

QSqlRecord CTDBConn::getPatientsData(QString id){

    qDebug() << "***********setting the local variable of CTDBConn->patientsId: " << id;
    setPatientsId(id);
    QSqlQuery query6;
    query6.prepare("SELECT * FROM patients WHERE id = :id_patient");
    query6.bindValue(":id_patient", id);
    query6.setForwardOnly(true);
    query6.exec();
    if(query6.next() && query6.size() ==1 ){
        QSqlRecord result = query6.record();
        return result;

    }else{qDebug()<< "Operation not succesfull" << query6.lastError(); return QSqlRecord();}

}

QStringList CTDBConn::getListofPatientsIds(){

    QStringList allPatientsIds;
    QSqlQuery query;
    query.prepare("SELECT id FROM patients");
    if(query.exec()){
        while(query.next()){
            allPatientsIds.append(query.value(0).toString());
        }
        return allPatientsIds;
    }
    else{qDebug() << "there was a problem in retreiving the patient's ids!";return QStringList();}
}

bool CTDBConn::savePatient(QHash<QString, QString> hashValuesLineEdit){
    QSqlQuery query10;
    query10.prepare("INSERT INTO patients(firstname,lastname,parent_1,parent_2,sex,attendant,address,zip_code,city,phone,email,notes,date_of_birth,id,gest_age) "
                    "VALUES(:firstname,:lastname,:parent_1,:parent_2,:sex,:attendant,:address,:zip_code,:city,:phone,:email,:notes,:date_of_birth,:id_patient,:gest_age)");
    query10.bindValue(":firstname",hashValuesLineEdit["firstname"]);
    query10.bindValue(":lastname", hashValuesLineEdit["lastname"]);
    query10.bindValue(":parent_1", hashValuesLineEdit["parent_1"]);
    query10.bindValue(":parent_2", hashValuesLineEdit["parent_2"]);
    query10.bindValue(":sex",hashValuesLineEdit["sex"]);
    query10.bindValue(":attendant", hashValuesLineEdit["attendant"]);
    query10.bindValue(":address", hashValuesLineEdit["address"]);
    if(hashValuesLineEdit["zip_code"].compare("") == 0){query10.bindValue(":zip_code",0);}
    else{query10.bindValue(":zip_code", hashValuesLineEdit["zip_code"]);}
    query10.bindValue(":city",hashValuesLineEdit["city"]);
    query10.bindValue(":phone", hashValuesLineEdit["phone"]);
    query10.bindValue(":email", hashValuesLineEdit["email"]);
    query10.bindValue(":notes", hashValuesLineEdit["notes"]);
    query10.bindValue(":date_of_birth",hashValuesLineEdit["date_of_birth"]);
//    query10.bindValue(":date_of_birth", QDate::fromString(hashValuesLineEdit["date_of_birth"],"ddd MMM d yyyy"));
    query10.bindValue(":id_patient", hashValuesLineEdit["id_patient"]);
    query10.bindValue(":gest_age", hashValuesLineEdit["gest_age"]);
    if(query10.exec()){return true;}
    else{qDebug() << "there was an error while executing the query : "<<query10.lastQuery() <<"the error:" <<query10.lastError();return false;}
}

bool CTDBConn::savePatientsData(QHash<QString, QString> hashValuesLineEdit){

    QSqlQuery query7;
    query7.prepare("UPDATE patients SET firstname = :firstname , lastname = :lastname, parent_1 = :parent_1, parent_2 = :parent_2, sex = :sex, attendant = :attendant ,address = :address, zip_code = :zip_code, city = :city, phone= :phone, email= :email, notes= :notes,date_of_birth = :date_of_birth,gest_age = :gest_age WHERE id = :id_patient");
    query7.bindValue(":firstname",hashValuesLineEdit["firstname"]);
    query7.bindValue(":lastname", hashValuesLineEdit["lastname"]);
    query7.bindValue(":parent_1", hashValuesLineEdit["parent_1"]);
    query7.bindValue(":parent_2", hashValuesLineEdit["parent_2"]);
    query7.bindValue(":sex",hashValuesLineEdit["sex"]);
    query7.bindValue(":attendant", hashValuesLineEdit["attendant"]);
    query7.bindValue(":address", hashValuesLineEdit["address"]);
    if(hashValuesLineEdit["zip_code"].compare("") == 0){query7.bindValue(":zip_code",0);}else
    {query7.bindValue(":zip_code", hashValuesLineEdit["zip_code"]);}
    query7.bindValue(":city",hashValuesLineEdit["city"]);
    query7.bindValue(":phone", hashValuesLineEdit["phone"]);
    query7.bindValue(":email", hashValuesLineEdit["email"]);
    query7.bindValue(":notes", hashValuesLineEdit["notes"]);
    query7.bindValue("date_of_birth",hashValuesLineEdit["date_of_birth"]);
//    query7.bindValue(":date_of_birth", QDate::fromString(hashValuesLineEdit["date_of_birth"],"ddd MMM d yyyy"));
    query7.bindValue(":gest_age",hashValuesLineEdit["gest_age"]);
    query7.bindValue(":id_patient", hashValuesLineEdit["id_patient"]);
    if(query7.exec()){return true;}
    else{qDebug() << "there was an error while executing the query : "<<query7.lastQuery() <<"the error:" <<query7.lastError();return false;}

}

bool CTDBConn::deletePatient(QString id){

    QSqlQuery query;
    QStringList relative_scenarios;
    query.prepare("SELECT relative_scenario FROM scenariosforpatient WHERE patient = :patient");
    query.bindValue(":patient",id);
    if(query.exec()){
        while(query.next()){
            relative_scenarios.append(query.value(0).toString());
        }
        qDebug() << "#######the extracted scenarios relative to the patient to be elimiminated are :" << relative_scenarios.size();
    }

    QSqlQuery query14;
    query14.prepare("DELETE FROM scenariosforpatient WHERE patient= :id_patient");
    query14.bindValue(":id_patient",id);
    if(query14.exec()){qDebug() << "Patient deleted from the scenariosforpatient table!";}
    else{qDebug()<< "Delete not successful!" << query14.lastError();}

    QSqlQuery query8;
    query8.prepare("DELETE FROM patients WHERE id = :id_patient");
    query8.bindValue(":id_patient",id);
    if(query8.exec()){qDebug() << "Patient deleted from patients table!";}
    else{qDebug()<< "Delete not successful!" << query8.lastError();}

    for(int i = 0; i <relative_scenarios.size();i ++){
        QSqlQuery query2;
        query2.prepare("DELETE FROM scenarios WHERE id = :id_scenario");
        query2.bindValue(":id_scenario",relative_scenarios.at(i));
        if(query2.exec()){qDebug() << "Relative scenarios of the deleted patient, also deleted from scenarios Table";return true;}
        else{qDebug() << "what the hell" << query2.lastError();return false;}
    }


}

QStringList CTDBConn::getScenarios(QString id){

    QSqlQuery query9;
    QStringList scenarios;
    query9.prepare("SELECT relative_scenario FROM scenariosforpatient WHERE patient= :id_patient");
    query9.bindValue(":id_patient", id);
    if(query9.exec()){
        while(query9.next()){
            scenarios.append(query9.value(0).toString());
        }
        return scenarios;
    }
    else{qDebug() << "There was an error!"  << query9.lastError();return QStringList();}
}

QString CTDBConn::getLastScenarioId(){
    QSqlQuery query15;
    QString scenario_id;
    query15.prepare("SELECT id FROM scenarios ORDER BY id");
    query15.exec();
    while(query15.next()){
        scenario_id = query15.value(0).toString();
        qDebug() << scenario_id;}
    return scenario_id;
}


bool CTDBConn::saveScenario(QHash <QString,QString> hashValuesScenario){
    QSqlQuery query11;
    query11.prepare("INSERT INTO scenarios(id,description,last_editor,last_edited,creation_date,total_runtime,scenario_blocks) "
                    "VALUES(:id_scenario,:description,:last_editor,:last_edited,:creation_date,:total_runtime,:scenario_blocks)");
    query11.bindValue(":id_scenario",hashValuesScenario["id_scenario"]);
    query11.bindValue(":description",hashValuesScenario["description"]);
    query11.bindValue(":last_editor", hashValuesScenario["last_editor"]);
//    QDateTime last_edited = QDateTime::fromString(hashValuesScenario["last_edited"]);
    query11.bindValue(":last_edited", hashValuesScenario["last_edited"]);
//    QDate creation_date = QDate::fromString(hashValuesScenario["creation_date"]);
    query11.bindValue(":creation_date", hashValuesScenario["creation_date"]);
//    QTime total_runtime = QTime::fromString(hashValuesScenario["total_runtime"]);
    query11.bindValue(":total_runtime", hashValuesScenario["total_runtime"]);
    query11.bindValue(":scenario_blocks",hashValuesScenario["scenario_blocks"]);
    if(query11.exec()){
        qDebug() << "insertion of the new scenario into DB successful!";
        QSqlQuery query12;
        query12.prepare("INSERT INTO scenariosforpatient(patient,relative_scenario)"
                        "VALUES(:patient,:relative_scenario)");
        qDebug() << "############# id_patient###########" << hashValuesScenario["patient"];
        query12.bindValue(":patient",hashValuesScenario["patient"]);
        query12.bindValue(":relative_scenario",hashValuesScenario["id_scenario"]);
        if(query12.exec()){qDebug() << "SUCCESS!";return true;}else{qDebug() << "Something went wrong!";}
    }
        else{qDebug()<< "There was an error saving the scenario in the DB!" << query11.lastError();return false;}
}

bool CTDBConn::updateScenario(QHash <QString,QString> hashValuesScenario){

    QSqlQuery query12;
    query12.prepare("UPDATE scenarios SET description= :description, last_editor= :last_editor, last_edited= :last_edited,scenario_blocks = :scenario_blocks WHERE id_scenario=:id_scenario");
    query12.bindValue(":description",hashValuesScenario["description"]);
    qDebug() << "new value of last_edited: " << hashValuesScenario["last_edited"];
//    QDateTime last_edited = QDateTime::fromString(hashValuesScenario["last_edited"]);
    query12.bindValue(":last_edited",hashValuesScenario["last_edited"]);
    query12.bindValue(":last_editor",hashValuesScenario["last_editor"]);
    query12.bindValue(":scenario_blocks",hashValuesScenario["scenario_blocks"]);
    qDebug()<<"id_scenario to be updated:" << hashValuesScenario["id_scenario"];
    query12.bindValue(":id_scenario",hashValuesScenario["id_scenario"]);
    if(query12.exec()){qDebug() << "Scenario updated successfully!";}else{qDebug() << "There was a problem updating the scenario" << query12.lastError();}
}

bool CTDBConn::deleteScenario(QString patientId,QString scenario_id){
    QSqlQuery query16;
    query16.prepare("DELETE FROM scenariosforpatient WHERE patient = :patient AND relative_scenario = :relative_scenario");
    query16.bindValue(":patient", patientId);
    query16.bindValue(":relative_scenario",scenario_id);
    if(query16.exec()){
        QSqlQuery query17;
        query17.prepare("DELETE FROM scenarios WHERE id_scenario = :id_scenario");
        query17.bindValue(":id_scenario", scenario_id);
        if(query17.exec()){
            qDebug() << "scenario successfully deleted!";
            return true;}
    }else{return false;}
}


QString CTDBConn::getScenarioXML(QString id_scenario){

    idScenario = id_scenario;
    QSqlQuery query13;
    query13.prepare("SELECT scenario_blocks FROM scenarios WHERE id_scenario= :id_scenario");
    query13.bindValue(":id_scenario", id_scenario);
    if(query13.exec()){if(query13.next())return query13.value(0).toString();}else{qDebug() << "There was an error retreiving the XML description of the Scenario";}

}

void CTDBConn::setPatientsId(QString id){
    qDebug() << "Storing patients id for further use:" << id;
    patientsId.clear();
    patientsId = id;
}

QString CTDBConn::getPatientsId(){
    return this->patientsId;
}

QString CTDBConn::getIdScenario(){
    return this->idScenario;
}

QSqlDatabase CTDBConn::getConnection(){
    QSqlDatabase db = QSqlDatabase::database();
    return db;
}

void CTDBConn::closeConnection(){
    QSqlDatabase db = QSqlDatabase::database();
    db.close();
}
