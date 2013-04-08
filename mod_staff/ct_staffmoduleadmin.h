#ifndef CT_STAFFMODULEADMIN_H
#define CT_STAFFMODULEADMIN_H

#include <QObject>
#include "ct_staffmodule.h"
#include "ct_tableofpatients.h"
#include "CareToy_Admin/ct_defs.h"
#include "ct_treeofworklogs.h"
#include "ct_worklogswidget.h"


class CTStaffModuleAdmin : public QWidget
{
    Q_OBJECT
public:
    explicit CTStaffModuleAdmin(QWidget *parent = 0);

    CTStaffModule *staffModule;

    void initialize();
    void refresh();
    void requestPatientsTable();

    void showStaffModule();
    void showConfirmationMessageStatus();
    void showFailureMessageStatus();
    void showWarningMessage(QString mssg);
    
private:
    CTTableOfPatients *tableOfPatients;
    CTWorklogsWidget *workLogs;

signals:
    void editSelectedPatient(QHash<QString,QString> patientToEdit);
    void openNewPatientDialog(QStringList refKeysList);
    void requestToWriteIntoSocket(const QString &parsedQuery,
                                  const quint32 &type);

public slots:
    QHash<QString, QString> getSelectedPatient();
    void requestEdit();
    void requestNew();
    void deleteSelectedPatient();
    void updateSelectedPatient(QHash<QString,QString> patientEdited);
    void saveNewPatient(QHash<QString,QString> newPatient);


    void proccessData(QByteArray table_data, QString table_name);

private slots:
    void execParsedQuery(QString initStmt, QString whereStmt, Data type_of_data);
    void requestWorkLog();
    void showMessageStatus(QString message);
    void afterTableInit();

};

#endif // CT_STAFFMODULEADMIN_H
