#ifndef CT_STAFFMODULEADMIN_H
#define CT_STAFFMODULEADMIN_H

#include <QObject>
#include "ct_staffmodule.h"
#include "ct_tableofpatients.h"

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

    void showStaffModule();
    void showConfirmationMessageStatus();
    
private:
    CTTableOfPatients *tableOfPatients;
    CTWorklogsWidget *workLogs;

signals:
    void editSelectedPatient(QHash<QString,QString> patientToEdit);
    void openNewPatientDialog(QStringList idList);
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
    void execParsedQuery(QString initStmt, QString whereStmt);
    void requestWorkLog();
    void showMessageStatus(QString message);

};

#endif // CT_STAFFMODULEADMIN_H
