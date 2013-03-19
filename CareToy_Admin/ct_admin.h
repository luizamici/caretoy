#ifndef CT_ADMIN_H
#define CT_ADMIN_H

#include <QObject>
#include "ct_dbconnadmin.h"
#include "ct_loginadmin.h"
#include "mod_staff/ct_staffmoduleadmin.h"
#include "Patients_Admin/ct_patientmoduleadmin.h"
#include "Patients_Admin/ct_qsqltableofscenarios.h"
#include "Patients_Admin/ct_qsqltableofresults.h"


class CTScenariosAdmin : public QObject
{
    Q_OBJECT
public:
    explicit CTScenariosAdmin(QObject *parent = 0);
    CTDBConnAdmin *dbConnAdmin;
    CTLoginAdmin *loginAdmin;
    CTStaffModuleAdmin *staffModuleAdmin;
    CTPatientModuleAdmin *patientModuleAdmin;
    CTQSqlTableOfPatients *sqlTableOfPatient;
    CTQSqlTableOfScenarios *sqlTableOfScenarios;
    CTQSqlTableOfResults *sqlTableOfResults;


private:
    QHash<QString,QString> userDataSession;
    void initializeStaffModule();

signals:
    
public slots:
    void loginSuccessful(QHash<QString,QString> userData);
    void refreshStaffModule(QString submodule);

    void editSelectedPatient(QHash<QString,QString> selectedPatient);

};

#endif // CT_ADMIN_H
