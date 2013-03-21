#ifndef CT_ADMIN_H
#define CT_ADMIN_H

#include <QObject>
#include "ct_dbconnadmin.h"
#include "ct_loginadmin.h"
#include "mod_staff/ct_staffmoduleadmin.h"
#include "Patients_Admin/ct_patientmoduleadmin.h"
#include "Patients_Admin/ct_qsqltableofresults.h"
#include "SecureSocketClient/ct_sslclientthread.h"


class CTAdmin : public QObject
{
    Q_OBJECT
public:
    explicit CTAdmin(QObject *parent = 0);
    CTDBConnAdmin *dbConnAdmin;
    CTLoginAdmin *loginAdmin;
    CTStaffModuleAdmin *staffModuleAdmin;
    CTPatientModuleAdmin *patientModuleAdmin;
    CTQSqlTableOfResults *sqlTableOfResults;


private:
    CTSslClientThread *sslClientThread;
    void initializeStaffModule();

private slots:
    void authenticate(QString username, QString psswd);
    void processXML(QByteArray data);
    void proccessData(QByteArray data, QString table_name);
    
public slots:
    void loginSuccessful();

    void editSelectedPatient(QHash<QString,QString> selectedPatient);

};

#endif // CT_ADMIN_H
