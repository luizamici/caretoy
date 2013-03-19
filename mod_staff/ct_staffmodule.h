#ifndef CT_STAFFMODULE_H
#define CT_STAFFMODULE_H

#include <QWidget>
#include <QtGui>
#include "ct_tableofpatients.h"
#include "CareToy_Admin/ct_dbconn.h"
#include "ct_worklog.h"
#include "Patients_Admin/ct_patientmodule.h"
#include "ct_searchpatient.h"

class CTStaffModule : public QWidget
{
    Q_OBJECT
public:
    explicit CTStaffModule(QHash<QString,QString> sessionData, QWidget *parent);

    QPushButton *editButton;
    QPushButton *deleteButton;
    QGridLayout *subLayout2;
    QVBoxLayout *subLayout3;
    QStatusBar *statusBar;

    CTSearchPatient* getSearchWidget();

signals:
    void editSelectedPatient();
    void openNewPatientDialog();
    void deleteSelectedPatient();
    void submit();

private:

    QHash<QString,QString> openSessionData;
    CTTableOfPatients *tableOfPatients;
    CTPatientModule *patientModuleById;
    CTSearchPatient *searchPatient;
    QMessageBox *confirmationMessage; 

public slots:
    void confirmDeletionOfpatient();
    
};

#endif // CT_STAFFMODULE_H
