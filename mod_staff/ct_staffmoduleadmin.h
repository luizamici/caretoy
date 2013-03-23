#ifndef CT_STAFFMODULEADMIN_H
#define CT_STAFFMODULEADMIN_H

#include <QObject>
#include "ct_staffmodule.h"
#include "ct_tableofpatients.h"
#include "ct_worklogs.h"

#include "ct_treeofworklogs.h"
#include "ct_worklogswidget.h"


class CTStaffModuleAdmin : public QWidget
{
    Q_OBJECT
public:
    explicit CTStaffModuleAdmin(QWidget *parent = 0);

    CTStaffModule *staffModule;

    void initialize();
    void requestTable();


    void setWorkLogList(QMap<QPair<QString, QString>, QString> workLogsListFromDB);
    void refreshWorkLogList(QMap<QPair<QString, QString>, QString> workLogsListFromDB, bool newLog);
    void showStaffModule();

    void showConfirmationMessageStatus();
    
private:

    QMap<QPair<QString,QString>,QString> localWorkLogsList;
    QStringList timestamps;
    QStringList logs;
    CTTableOfPatients *tableOfPatients;
    CTWorklogsWidget *workLogs;

    QString localSelectedLogTimestamp;

    bool isNewLog();
    void sortLocalWorkLogsListById();
    void showMessageStatus(QString message);
signals:
    void saveNewLog(QHash<QString,QString> newlogToSave);
    void updateLog(QHash<QString,QString> worklogToUpdate);
    void editSelectedPatient(QHash<QString,QString> patientToEdit);
    void openNewPatientDialog(QStringList idList);

    void requestToWriteIntoSocket(const QString &parsedQuery,
                                  const quint32 &type);

public slots:
    void getLog();
    void updateWorklogEditor(QString selectedlogtimestamp);

    QHash<QString, QString> getSelectedPatient();
    void requestEdit();
    void requestNew();
    void deleteSelectedPatient();
    void updateSelectedPatient(QHash<QString,QString> patientEdited);
    void saveNewPatient(QHash<QString,QString> newPatient);


    void proccessData(QByteArray table_data, QString table_name);

private slots:
    void execParsedQuery(QString initStmt, QString whereStmt);
    void requestWorkLog();

};

#endif // CT_STAFFMODULEADMIN_H
