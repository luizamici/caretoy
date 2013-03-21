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
    explicit CTStaffModule(QWidget *parent);

    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *addButton;
    QGridLayout *subLayout2;
    QVBoxLayout *subLayout3;
    QStatusBar *statusBar;
    QLabel *label_1;
    QLabel *label_2;
    CTSearchPatient *searchPatient;

    void showOkMessage(QString message);
signals:
    void openNewPatientDialog();
    void deleteSelectedPatient();

private:
    QMessageBox *confirmationMessage; 

public slots:
    void confirmDeletionOfpatient();
    
};

#endif // CT_STAFFMODULE_H
