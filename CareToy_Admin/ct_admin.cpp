#include "ct_admin.h"
#include "mod_staff/ct_staffmodule.h"

CTAdmin::CTAdmin(QObject *parent) :
    QObject(parent)
{
    loginAdmin = new CTLoginAdmin();
    sslClientThread  = new CTSslClientThread();
    sslClientThread->run();

    /*
     *This signal is emitted whenever the user presses 'ok' button on the login dialog
     *The dbConnAdmin controls if the username and password inserted are present in the db
     */
    connect(loginAdmin,SIGNAL(requestForAuthentication(QString,QString)),this,
            SLOT(authenticate(QString,QString)));

    connect(sslClientThread,SIGNAL(processXML(QByteArray)),this, SLOT(processXML(QByteArray)));
//    reconnectTimeOuts = 0;

//    connect(sslClientThread, SIGNAL(connectionSuccessful(QString)), dialog,
//            SLOT(showMessage(QString)));
//    connect(sslClientThread,SIGNAL(notConnected(QString)),this, SLOT(
//                connectionLost(QString)));

    dbConnAdmin = new CTDBConnAdmin();
    staffModuleAdmin = new CTStaffModuleAdmin();
    patientModuleAdmin = new CTPatientModuleAdmin();


    connect(staffModuleAdmin, SIGNAL(requestToWriteIntoSocket(QString,quint32)),
            sslClientThread, SLOT(requestTable(QString,quint32)));

    connect(patientModuleAdmin, SIGNAL(requestToWriteIntoSocket(QString,quint32)),
            sslClientThread, SLOT(requestTable(QString,quint32)));

    connect(sslClientThread,SIGNAL(processTable(QByteArray,QString)),this,
                SLOT(proccessData(QByteArray,QString)));

    /*
     *In case the staffModuleAdmin is requesting a new worklog to be stored to the DB
     */
    connect(staffModuleAdmin,SIGNAL(saveNewLog(QHash<QString,QString>)),dbConnAdmin,SLOT(saveNewLog(QHash<QString,QString>)));
    connect(staffModuleAdmin,SIGNAL(updateLog(QHash<QString,QString>)),dbConnAdmin,SLOT(updateLog(QHash<QString,QString>)));
    connect(staffModuleAdmin,SIGNAL(editSelectedPatient(QHash<QString,QString>)),this,SLOT(editSelectedPatient(QHash<QString,QString>)));
    connect(staffModuleAdmin,SIGNAL(openNewPatientDialog(QStringList)),patientModuleAdmin,SLOT(openNewPatientDialog(QStringList)));

    connect(patientModuleAdmin,SIGNAL(selectedPatientChanged(QHash<QString,QString>)),staffModuleAdmin,SLOT(updateSelectedPatient(QHash<QString,QString>)));
    connect(patientModuleAdmin,SIGNAL(newPatientAdded(QHash<QString,QString>)),staffModuleAdmin,SLOT(saveNewPatient(QHash<QString,QString>)));

}

void CTAdmin::authenticate(QString username, QString psswd)
{
    QCryptographicHash *crypt = new QCryptographicHash(QCryptographicHash::Sha1);
    QByteArray hashedPsswd = crypt->hash(psswd.toLatin1(),QCryptographicHash::Sha1);
    QString hashedPsswdToString = hashedPsswd.toHex().constData();

    QString statement;
    QXmlStreamWriter stream(&statement);
    stream.writeStartElement("login_request");
    stream.writeAttribute("username", username);
    stream.writeAttribute("password", hashedPsswdToString);
    stream.writeEndElement();//end login_request

    sslClientThread->writeIntoSocket(statement, CT_PKTDATA);
    return;
}


void CTAdmin::processXML(QByteArray data)
{
    QXmlStreamReader reader(data);
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement() && !reader.isStartDocument())
        {
            QString tagName = reader.name().toString();
            if ("login_reply" == tagName)
            {
                QXmlStreamAttributes attr = reader.attributes();
                QString type = attr.value("message").toString();
//                QString name = attr.value("name").toString();
//                QString surname = attr.value("surname").toString();
//                QString id = attr.value("id").toString();
//                QString last_login = attr.value("last_login").toString();
                if("success" == type)
                {
                    qDebug() << "SUCCESS";
                    qApp->setProperty("UserName", "Name");
                    qApp->setProperty("UserSurname", "Surname");
                    qApp->setProperty("UserID", "2");
                    qApp->setProperty("UserLastLogin", "last_login");
                    loginSuccessful();
                }
                else if("failure" == type)
                    qDebug() << "Failure!";
            }
            else if("query_reply" == tagName)
            {
                QXmlStreamAttributes attr = reader.attributes();
                QString type = attr.value("message").toString();
                if("success" == type)
                {
                    initializeStaffModule();
                    staffModuleAdmin->showConfirmationMessageStatus();
                    patientModuleAdmin->showConfirmationMessageStatus();
                }
            }
            else
                qDebug() << "Reply not recognized";
        }
    }
    return;
}

void CTAdmin::proccessData(QByteArray data, QString table_name)
{
    if(table_name == "patients" || table_name == "worklogs")
        staffModuleAdmin->proccessData(data,table_name);
    else if(table_name == "test_scenario" )
        patientModuleAdmin->proccessData(data, table_name);
}


/*
 *The user interface switches from the login dialog to the staff module window in case of successful login
 */
void CTAdmin::loginSuccessful(){

    loginAdmin->close();
    initializeStaffModule();
}


void CTAdmin::initializeStaffModule(){
    staffModuleAdmin->initialize();
    staffModuleAdmin->showStaffModule();
}


void CTAdmin::editSelectedPatient(QHash<QString, QString> selectedPatient){
    patientModuleAdmin->initEdit(selectedPatient);
}
