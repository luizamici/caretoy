#ifndef CT_STAFFMODULEADMIN_H
#define CT_STAFFMODULEADMIN_H

#include <QObject>
#include "ct_staffmodule.h"
#include "ct_tableofpatients.h"
#include "ct_worklogs.h"
#include "ct_qsqltableofpatients.h"

class CTStaffModuleAdmin : public QWidget
{
    Q_OBJECT
public:
    explicit CTStaffModuleAdmin(QWidget *parent = 0);

    CTStaffModule *staffModule;
    CTQSqlTableOfPatients *sqlTableModelOfPatients;

    void initialize(QHash<QString, QString> sessionData);
    void setSqlTableModelOfPatients(CTQSqlTableOfPatients *sqlTableModelOfPatientsFromDB);
    void setWorkLogList(QMap<QPair<QString, QString>, QString> workLogsListFromDB);
    void refreshWorkLogList(QMap<QPair<QString, QString>, QString> workLogsListFromDB, bool newLog);
    void showStaffModule();

    void showConfirmationMessageStatus();
    
private:
    QHash<QString,QString> localSessionData; //contains the user data:
                                             //id_staff(id of user inside the DB staff's table),
                                             //name, surname
    QMap<QPair<QString,QString>,QString> localWorkLogsList;
    QStringList timestamps;
    QStringList logs;
    CTTableOfPatients *tableOfPatients;
    CTWorklogs *workLogs;
    QString localSelectedLogTimestamp;

    bool isNewLog();
    void sortLocalWorkLogsListById();
    void showMessageStatus(QString message);
signals:
    void saveNewLog(QHash<QString,QString> newlogToSave);
    void updateLog(QHash<QString,QString> worklogToUpdate);

    void editSelectedPatient(QHash<QString,QString> patientToEdit);
    void openNewPatientDialog(QStringList idList);

    void error(QString error);
    void success(QString message,QString row);
    
    void insertUser(QSqlRecord& newUser);

public slots:
    void getLog();
    void updateWorklogEditor(QString selectedlogtimestamp);

    QHash<QString, QString> getSelectedPatient();
    void requestEdit();
    void requestNew();
    void deleteSelectedPatient();
    void updateSelectedPatient(QHash<QString,QString> patientEdited);
    void saveNewPatient(QHash<QString,QString> newPatient);

};

#endif // CT_STAFFMODULEADMIN_H
