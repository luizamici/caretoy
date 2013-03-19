#ifndef CT_PATIENTMODULEADMIN_H
#define CT_PATIENTMODULEADMIN_H

#include <QWidget>
#include "ct_patientmodule.h"
#include "ct_wizard.h"
#include "ct_tableofscenarios.h"
#include "ct_tableofresults.h"

class CTPatientModuleAdmin : public QWidget
{
    Q_OBJECT
public:
    explicit CTPatientModuleAdmin(QWidget *parent = 0);

    void setSqlTableModelOfScenarios(CTQSqlTableOfScenarios *sqlTableModelOfScenariosFromDB);
    
private:
    CTPatientModule *patientModule;
    bool newPatient;
    QHash<QString,QString> localPatientData;
    CTPatientsForm *form;
    CTTableOfScenarios *tableOfScenarios;
    CTTableOfResults *tableOfResults;
    QMessageBox *confirmationMessage;

    bool isEmpty(QHash<QString,QString> p);

    void updateLocalPatientData(QHash<QString,QString> patientDataFromUI);


signals:
    void selectedPatientChanged(QHash<QString,QString> patient);
    void newPatientAdded(QHash<QString,QString> new_patient);

public slots:
    void editSelectedPatient(QHash<QString,QString> patient, CTQSqlTableOfScenarios *sqlTableOfpatients,CTQSqlTableOfResults *sqlTableOfResults);
    void openNewPatientDialog(QStringList idList);

    void goBack();
    void updateRow(QString message,QString row);
    void searchResults(QString id_scenario);
    void checkAddButtonClicked();

    void showErrorMessage(QString errorMessage);
    void showConfirmationDialog();
};

#endif // CT_PATIENTMODULEADMIN_H
