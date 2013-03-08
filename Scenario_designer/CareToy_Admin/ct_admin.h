#ifndef CT_ADMIN_H
#define CT_ADMIN_H

#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QTimer>
#include <QCryptographicHash>
#include "ct_logindialog.h"
#include "SecureSocketClient/ct_sslclientthread.h"
#include "TableOfScenariosFromXML/ct_scenariosadmin.h"


class CTAdmin : public QObject
{
    Q_OBJECT
public:
    CTAdmin();

private:

    CTLoginDialog *dialog;
    CTSslClientThread *sslClientThread;
    CTScenariosAdmin *scenarioAdmin;

    int reconnectTimeOuts;

public slots:
    void connectionLost(QString mss);

private slots:
    void authenticate(QString username, QString psswd);
    void timerTimeout();
    void processXML(QByteArray data);
};

#endif // CT_ADMIN_H
