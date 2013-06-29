#include "ct_admin.h"
#include "ct_defs.h"

CTAdmin::CTAdmin()
{
    dialog = new CTLoginDialog();
    dialog->show();

    sslClientThread  = new CTSslClientThread();
    sslClientThread->run();
    reconnectTimeOuts = 0;

    scenarioAdmin = new CTScenariosAdmin();

    connect(sslClientThread, SIGNAL(connectionSuccessful(QString)), dialog,
            SLOT(showMessage(QString)));
    connect(sslClientThread,SIGNAL(notConnected(QString)),this, SLOT(
                connectionLost(QString)));
    connect(dialog, SIGNAL(credentials(QString,QString)), this, SLOT(
                authenticate(QString,QString)));

    connect(sslClientThread,SIGNAL(processData(QByteArray)),scenarioAdmin,
            SLOT(proccessData(QByteArray)));
    connect(sslClientThread,SIGNAL(processXML(QByteArray)),this, SLOT(processXML(QByteArray)));

    connect(scenarioAdmin, SIGNAL(requestToWriteIntoSocket(QString,quint32)),
            sslClientThread, SLOT(requestTable(QString,quint32)));
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
    qDebug() << data;
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
                if("success" == type)
                {
                    dialog->close();
                    scenarioAdmin->initialize();
                }
                else if("failure" == type)
                {
                    qDebug() << attr.value("user_id").toString();
                    dialog->showMessage("Username or Password incorrect");
                }
            }
            else if("query_reply" == tagName)
            {
                QXmlStreamAttributes attr = reader.attributes();
                QString type = attr.value("message").toString();
                if("success" == type)
                {
                    scenarioAdmin->showMessage("Query executed successfully!");
                    scenarioAdmin->requestTable();
                }
            }
            else
                qDebug() << "Reply not recognized";
        }
    }
    return;
}


void CTAdmin::connectionLost(QString mss)
{
    int i = QMessageBox::critical(0, tr("CareToy Admin"),mss);
    if(dialog->isVisible())
        dialog->statusBar->showMessage("Not connected to server. "
                                     "Trying to reconnect ...");
    QTimer::singleShot(5000, this, SLOT(timerTimeout()));
}

void CTAdmin::timerTimeout()
{
    if(reconnectTimeOuts < 5)
        sslClientThread->initialize();
    else
        if(dialog->isVisible())
            dialog->statusBar->showMessage("Impossible to establish a connection "
                                         "with the server!");
    reconnectTimeOuts++;
}

