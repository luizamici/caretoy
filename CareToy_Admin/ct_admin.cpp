#include "ct_admin.h"
#include "mod_staff/ct_staffmodule.h"
#include "ct_aux.h"

CTAdmin::CTAdmin(QObject *parent) :
    QObject(parent)
{
    loginAdmin = new CTLoginAdmin();
    sslClientThread  = new CTSslClientThread();
    sslClientThread->run();

    reconnectTimeOuts = 0;
    qApp->setProperty("dirty_data", T_NoOne);


    connect(loginAdmin,SIGNAL(requestForAuthentication(QString,QString)),this,
            SLOT(authenticate(QString,QString)));

    connect(sslClientThread,SIGNAL(processXML(QByteArray)),this, SLOT(processXML(QByteArray)));

//    connect(sslClientThread, SIGNAL(connectionSuccessful(QString)), dialog,
//            SLOT(showMessage(QString)));
    connect(sslClientThread,SIGNAL(notConnected(QString)),this, SLOT(
                connectionLost(QString)));

    staffModuleAdmin = new CTStaffModuleAdmin();
    patientModuleAdmin = new CTPatientModuleAdmin();


    connect(staffModuleAdmin, SIGNAL(requestToWriteIntoSocket(QString,quint32)),
            sslClientThread, SLOT(requestTable(QString,quint32)));

    connect(patientModuleAdmin, SIGNAL(requestToWriteIntoSocket(QString,quint32)),
            sslClientThread, SLOT(requestTable(QString,quint32)));

    connect(sslClientThread,SIGNAL(processTable(QByteArray,QString)),this,
                SLOT(proccessData(QByteArray,QString)));


    connect(staffModuleAdmin,SIGNAL(editSelectedPatient(QHash<QString,QString>)),
            this,SLOT(editSelectedPatient(QHash<QString,QString>)));
    connect(staffModuleAdmin,SIGNAL(openNewPatientDialog(QStringList)),
            patientModuleAdmin,SLOT(openNewPatientDialog(QStringList)));

    connect(patientModuleAdmin,SIGNAL(selectedPatientChanged(QHash<QString,QString>)),
            staffModuleAdmin,SLOT(updateSelectedPatient(QHash<QString,QString>)));
    connect(patientModuleAdmin,SIGNAL(newPatientAdded(QHash<QString,QString>)),
            staffModuleAdmin,SLOT(saveNewPatient(QHash<QString,QString>)));

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
                QString id = attr.value("id").toString();
                QString name = attr.value("name").toString();
                QString lastname = attr.value("lastname").toString();
                QString last_login = formatDate(attr.value("last_login").toString());
                if("success" == type && !name.trimmed().isEmpty())
                {
                    qApp->setProperty("UserName", name);
                    qApp->setProperty("UserSurname", lastname);
                    qApp->setProperty("UserID", id);
                    qApp->setProperty("UserLastLogin", last_login);
                    loginSuccessful();
                }
                else if("failure" == type)
                {
                    loginAdmin->showWrongCredentialsMessage(
                                "Username or password incorrect!");

                }
                else
                {
                    loginAdmin->showWrongCredentialsMessage(
                                "Not an administrator username and password");
                }
            }
            else if("query_reply" == tagName)
            {
                QXmlStreamAttributes attr = reader.attributes();
                QString type = attr.value("message").toString();
                if("success" == type)
                {
                    refreshStaffModule();
                    staffModuleAdmin->showConfirmationMessageStatus();
                    patientModuleAdmin->showConfirmationMessageStatus();
                }
                else if("failure" == type)
                {
                    refreshStaffModule();
                    staffModuleAdmin->showFailureMessageStatus();
                    patientModuleAdmin->showFailureMessageStatus();
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
 *The user interface switches from the login dialog to the staff
 *module window in case of successful login
 */
void CTAdmin::loginSuccessful(){

    loginAdmin->close();
    initializeStaffModule();
}


void CTAdmin::initializeStaffModule(){
    staffModuleAdmin->initialize();
    staffModuleAdmin->showStaffModule();
}

void CTAdmin::refreshStaffModule()
{
    staffModuleAdmin->refresh();
}


void CTAdmin::editSelectedPatient(QHash<QString, QString> selectedPatient){
    patientModuleAdmin->initEdit(selectedPatient);
}


void CTAdmin::connectionLost(QString mss)
{
    int i = QMessageBox::critical(0, tr("CareToy Admin"),mss);

    staffModuleAdmin->showWarningMessage("Not connected to server. "
                                        "Trying to reconnect ...");
    patientModuleAdmin->showMessage("Not connected to server. "
                                    "Trying to reconnect ...");
    QTimer::singleShot(5000, this, SLOT(timerTimeout()));
}

void CTAdmin::timerTimeout()
{
    if(reconnectTimeOuts < 5)
        sslClientThread->initialize();
    else
    {
        staffModuleAdmin->showWarningMessage("Impossible to establish a connection "
                                         "with the server!");
        patientModuleAdmin->showMessage("Impossible to establish a connection "
                                        "with the server!");
    }
    reconnectTimeOuts++;
}
