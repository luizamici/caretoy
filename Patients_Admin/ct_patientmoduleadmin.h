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

    void proccessData(QByteArray table_data);
    
private:
    CTPatientModule *patientModule;
    bool newPatient;
    QHash<QString,QString> localPatientData;
    CTPatientsForm *form;
    CTTableOfScenarios *tableOfScenarios;
    CTTableOfResults *tableOfResults;
    QMessageBox *confirmationMessage;

    void updateLocalPatientData(QHash<QString,QString> patientDataFromUI);

    void requestTableOfScenarios();
    void execParsedQuery(QString initStmt, QString whereStmt);



signals:
    void selectedPatientChanged(QHash<QString,QString> patient);
    void newPatientAdded(QHash<QString,QString> new_patient);

    void requestToWriteIntoSocket(const QString &parsedQuery,
                                  const quint32 &type);

public slots:
    void initEdit(QHash<QString,QString> patient);
    void openNewPatientDialog(QStringList idList);

    void goBack();
    void searchResults(QString id_scenario);
    void checkAddButtonClicked();

    void showConfirmationDialog();


    void showConfirmationMessageStatus();
};

#endif // CT_PATIENTMODULEADMIN_H
